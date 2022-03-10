AI = Class{}

-- magical init is 192 for number 0
-- magical init for number 1 is 205
-- MAGICAL intervals is 12
function AI:init()
    self.x = 192
    self.y = 40
    self.width = 9
    self.height = 9
end

function AI:render()
    love.graphics.rectangle('line', self.x, self.y, self.width, self.height)
end

function AI:delete()
    self.x = -10
end