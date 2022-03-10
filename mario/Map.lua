--[[
    Contains tile data and necessary code for rendering a tile map to the
    screen.
]]

require 'Util'

Map = Class{}

TILE_BRICK = 1
TILE_EMPTY = 4

-- cloud tiles
CLOUD_LEFT = 6
CLOUD_RIGHT = 7

-- bush tiles
BUSH_LEFT = 2
BUSH_RIGHT = 3

-- mushroom tiles
MUSHROOM_TOP = 10
MUSHROOM_BOTTOM = 11

-- jump block
JUMP_BLOCK = 5
JUMP_BLOCK_HIT = 9

-- flagpole
FLAG_POLE_TOP = 8
FLAG_POLE_MIDDLE = 12
FLAG_POLE_BOTTOM = 16

-- flag
FLAG_1 = 13
FLAG_2 = 14
FLAG_3 = 15

-- a speed to multiply delta time to scroll map; smooth value
local SCROLL_SPEED = 62

-- constructor for our map object
function Map:init()

    self.spritesheet = love.graphics.newImage('graphics/spritesheet.png')
    self.sprites = generateQuads(self.spritesheet, 16, 16)
    self.music = love.audio.newSource('sounds/music.wav', 'static')

    self.tileWidth = 16
    self.tileHeight = 16
    self.mapWidth = 30 * 2
    self.mapHeight = 28
    self.tiles = {}

    -- applies positive Y influence on anything affected
    self.gravity = 15

    -- associate player with map
    self.player = Player(self)
    
    -- camera offsets
    self.camX = 0
    self.camY = -3

    -- cache width and height of map in pixels
    self.mapWidthPixels = self.mapWidth * self.tileWidth
    self.mapHeightPixels = self.mapHeight * self.tileHeight

    -- first, fill map with empty tiles
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            
            -- support for multiple sheets per tile; storing tiles as tables 
            self:setTile(x, y, TILE_EMPTY)
        end
    end

    -- Generate cloud across the map using vertical scan lines
    local cloud = 1
    while cloud < self.mapWidth do
        -- 2% chance to generate a cloud
        -- make sure we're 2 tiles from edge at least
        if cloud < self.mapWidth - 2 then
            if math.random(20) == 1 then
                
                -- choose a random vertical spot above where blocks/pipes generate
                local cloudStart = math.random(self.mapHeight / 2 - 6)

                self:setTile(cloud, cloudStart, CLOUD_LEFT)
                self:setTile(cloud + 1, cloudStart, CLOUD_RIGHT)
            end
        end
        cloud = cloud + 1
    end

    local x = 1 
    local gap = 0
    local ending = self.mapWidth - 20

    -- begin generating the terrain using vertical scan lines until the start of end
    while x < ending do

        -- 5% chance to generate a mushroom
        -- ensure that there is no gap before mushroom to facilitate jumping
        if math.random(20) == 1 and gap == 0 then
            -- left side of pipe
            self:setTile(x, self.mapHeight / 2 - 2, MUSHROOM_TOP)
            self:setTile(x, self.mapHeight / 2 - 1, MUSHROOM_BOTTOM)

            -- creates column of tiles going to bottom of map
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

            -- next vertical scan line
            x = x + 1
            gap = 0

        -- 10% chance to generate bush, being sure to generate away from edge
        elseif math.random(10) == 1 and x < self.mapWidth - 3 then
            local bushLevel = self.mapHeight / 2 - 1

            -- place bush component and then column of bricks
            self:setTile(x, bushLevel, BUSH_LEFT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            x = x + 1

            self:setTile(x, bushLevel, BUSH_RIGHT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            x = x + 1
            gap = 0
        
        -- 90% chance to generate the floor, and 15% chance to create a JUMP_BLOCK
        elseif math.random(10) ~= 1 then
            
            -- creates column of tiles going to bottom of map
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

            -- chance to create a block for Mario to hit
            if math.random(15) == 1 then
                self:setTile(x, self.mapHeight / 2 - 4, JUMP_BLOCK)
            end

            -- next vertical scan line
            x = x + 1
            gap = 0
    
        -- 10% chance to not generate anything, creating a 2/3 - tile gap
        else
            -- increment X so we skip two scanlines, creating a 2/3 - tile gap
            local r = math.random(2,3)
            for z = 0, r do
                
                -- Make sure gap is no bigger than 5 tiles
                if gap < 5 then
                    gap = gap + 1
                else
                    for y = self.mapHeight / 2, self.mapHeight do
                        self:setTile(x, y, TILE_BRICK)
                    end
                    gap = 0
                end
                x = x + 1
            end
        end
    end

    local count = 0
    -- Generate ending floor tiles, covering the whole area
    while ending < self.mapWidth do
        for y = self.mapHeight / 2, self.mapHeight do
            self:setTile(ending, y, TILE_BRICK)
        end
        
        -- Generate pyramid of height 8
        if count < 10 then
            local j = -1

            for i = 0, count do
                j = j + 1
            end
    
            for k = self.mapHeight/2 , self.mapHeight/2 - j, -1 do
                self:setTile(ending, k, TILE_BRICK)
            end
            
            count = count + 1
        end

        -- Generate flag pole 5 blocks away from the end
        if ending == self.mapWidth - 5 then
            local flagPole = 0
            while flagPole < self.mapHeight/2 do
                if flagPole == 5 then
                    self:setTile(ending, flagPole, FLAG_POLE_TOP)
                elseif flagPole < self.mapHeight / 2 - 1 and flagPole > 5 then
                    self:setTile(ending, flagPole, FLAG_POLE_MIDDLE)
                elseif flagPole == self.mapHeight/2 - 1 then
                    self:setTile(ending, flagPole, FLAG_POLE_BOTTOM)
                end
                flagPole = flagPole + 1     
            end   
        end
        
        ending = ending + 1
    end

    -- start the background music
    self.music:setLooping(true)
    self.music:play()
end

-- return whether a given tile is collidable
function Map:collides(tile)
    -- define our collidable tiles
    local collidables = {
        TILE_BRICK, JUMP_BLOCK, JUMP_BLOCK_HIT,
        MUSHROOM_TOP, MUSHROOM_BOTTOM, 
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(collidables) do
        if tile.id == v then
            return true
        end
    end

    return false
end

function Map:wins(tile)
    -- define our collidable tiles
    local winnables = {
        FLAG_POLE_BOTTOM, FLAG_POLE_MIDDLE, FLAG_POLE_TOP
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(winnables) do
        if tile.id == v then
            return true
        end
    end

    return false
end


-- function to update camera offset with delta time
function Map:update(dt)
    self.player:update(dt)
    
    -- keep camera's X coordinate following the player, preventing camera from
    -- scrolling past 0 to the left and the map's width
    self.camX = math.max(0, math.min(self.player.x - VIRTUAL_WIDTH / 2,
        math.min(self.mapWidthPixels - VIRTUAL_WIDTH - self.player.width, self.player.x)))
end

-- gets the tile type at a given pixel coordinate
function Map:tileAt(x, y)
    return {
        x = math.floor(x / self.tileWidth) + 1,
        y = math.floor(y / self.tileHeight) + 1,
        id = self:getTile(math.floor(x / self.tileWidth) + 1, math.floor(y / self.tileHeight) + 1)
    }
end

-- returns an integer value for the tile at a given x-y coordinate
function Map:getTile(x, y)
    return self.tiles[(y - 1) * self.mapWidth + x]
end

-- sets a tile at a given x-y coordinate to an integer value
function Map:setTile(x, y, id)
    self.tiles[(y - 1) * self.mapWidth + x] = id
end

-- renders our map to the screen, to be called by main's render
function Map:render()
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            local tile = self:getTile(x, y)
            if tile ~= TILE_EMPTY then
                love.graphics.draw(self.spritesheet, self.sprites[tile],
                    (x - 1) * self.tileWidth, (y - 1) * self.tileHeight)
            end
        end
    end

    self.player:render()
end