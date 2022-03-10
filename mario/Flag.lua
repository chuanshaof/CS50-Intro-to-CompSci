--[[
    Represents our player in the game, with its own sprite.
]]

Flag = Class{}

function Flag:init(map)
    
    self.x = 0
    self.y = 0
    self.width = 16
    self.height = 16

    -- offset from top left to center to support sprite flipping
    self.xOffset = 8
    self.yOffset = 10

    -- reference to map for checking tiles
    self.map = map
    self.texture = love.graphics.newImage('graphics/spritesheet.png')

    -- sound effects
    self.sounds = {
        ['win'] = love.audio.newSource('sounds/powerup-reveal.wav', 'static')
    }

    -- animation frames
    self.frames = {}

    -- current animation frame
    self.currentFrame = nil

    -- used to determine behavior and animations
    self.state = 'flag_still'

    -- x and y velocity
    self.dx = 0
    self.dy = 0

    -- position at position
    self.y = 4 * map.tileHeight
    self.x = (map.mapWidth - 5) * map.tileWidth

    -- initialize all flag animations
    self.animations = {
        ['flag_still'] = Animation({
            texture = self.texture,
            frames = {
                -- flag 1
                love.graphics.newQuad(0, 48, 16, 16, self.texture:getDimensions())
            }
        }),
        ['flag_fly'] = Animation({
            texture = self.texture,
            frames = {
                -- flag 2
                love.graphics.newQuad(16, 48, 16, 16, self.texture:getDimensions())
            }
        }),
        ['flag_down'] = Animation({
            texture = self.texture,
            frames = {
                -- flag 3
                love.graphics.newQuad(32, 48, 16, 16, self.texture:getDimensions())
            }
        })
    }

    -- initialize animation and current frame we should render
    self.animation = self.animations['flag_still']
    self.currentFrame = self.animation:getCurrentFrame()

    -- behavior map we can call based on player state
    self.behaviors = {
        ['flag_still'] = function(dt)
            self.animation = self.animations['flag_still']
        end,

        ['flag_fly'] = function(dt)
            self.animation = self.animations['flag_fly']
            self.dy = self.dy + self.map.gravity
            self.sounds['win']:play()
        end,

        ['flag_down'] = function(dt)
            self.dy = 0
            self.y = map.tileHeight * ((map.mapHeight - 2) / 2) - self.height
            self.animation = self.animations['flag_down']
        end
    }
end

function Flag:update(dt)
    self.behaviors[self.state](dt)
    self.animation:update(dt)
    self.currentFrame = self.animation:getCurrentFrame()

    -- apply velocity
    self.y = self.y + self.dy * dt
end

function Flag:render()
    -- draw flag at position
    love.graphics.draw(self.texture, self.currentFrame, math.floor(self.x + self.xOffset),
        math.floor(self.y + self.yOffset), 0, scaleX, 1, self.xOffset, self.yOffset)
end