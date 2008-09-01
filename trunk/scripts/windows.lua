require('scripts/widgets/basic')
require('scripts/common')
require(theme..'/common')
--[[
w1 = widget:newEx(desktop,window)
w2 = widget:newEx(desktop,window)
w12 = widget:newEx(w1,caption)
w11 = widget:newEx(w1,button)
w21 = widget:newEx(w2,caption)

function w1:setup()
	self.position.relative.left = 8
	self.position.relative.top = 8
	self.position.relative.width = 256
	self.position.relative.height = 320
	self.angle = angle
	self.pos = pos
	self.collisions = collisions
	self.caption = "Testing window"
	self.visible = true
	self.enabled = true
	self.minimalized = false
	widget.setup(self)
	self:update()
	self.stimer = 0
	self.style.title = true
	self.style.border = true
end

function w1:update()
	widget.update(self)
end

function w1:draw()
	widget.draw(self)
end

function w2:setup()
	self.position.relative.width = 256
	self.position.relative.height = 128
	self.position.relative.left = config.screen.width-self.position.relative.width-8
	self.position.relative.top = 8
	self.time = 0
	self.fps = 0
	self.caption = "Info window"
	self.visible = true
	self.enabled = true
	self.minimalized = false
	self.style.title = true
	self.style.border = true
	widget.setup(self)
	w2:update()
end

function w2:update()
	widget.update(self)
end

function w2:event(event, param1, param2)
	--window.event(self, event, param1, param2)
	return widget.event(self, event, param1, param2)
end

function w2:draw()
	widget.draw(self)
end

function w11:setup()
	self.position.relative.left = 8
	self.position.relative.top = self.parent.position.absolute.height-24-8
	self.position.relative.width = 128
	self.position.relative.height = 24
	self.caption = "Button 1"
	self.name = "Button"
	self.visible = true
	self.enabled = true
	self.minimalized = false
	self.static_children = true
	self.a = 0.7
	self.a2 = 0.7
	widget.setup(self)
	self:update()
end

function w11:update()
	widget.update(self)
	self.context = "Button 1"

end

function w11:draw()
	widget.draw(self)
	button.draw(self)
end

function w12:setup()
	self.position.relative.left = 0
	self.position.relative.top = 0
	self.position.relative.width = 250
	self.position.relative.height = 350
	self.caption = "Caption 1"
	self.name = "Caption"
	self.visible = true
	self.enabled = true
	self.minimalized = false
	self.static_children = true
	self.a = 0.7
	self.a2 = 0.7
    self.format = "button: %X\nlast_button: %X\nedge_test: %X\nclick: %d\nMouse [%d,%d]\nRMouse [%d,%d]\nSelf: [%d,%d]\nFocused window:\n %s\nOver window:\n %s\nResize window: %s\nPos [%d,%d,%d]\n"
	widget.setup(self)
	self:update()
end

function w12:update()
	widget.update(self)
	if (type(resize_window)=="table") then
		rw = resize_window.caption
	else                  
		rw = "None"
	end
    self.context = string.format(self.format,
    	data.mouse.button,
    	data.mouse.last_button,
    	edge_test,
    	(data.mouse.click and 1) or 0,
    	data.mouse.x,
    	data.mouse.y,
    	data.mouse.rx,
    	data.mouse.ry,
    	self.position.absolute.left,
    	self.position.absolute.top,
    	tostring(grab.focused_window and grab.focused_window.caption),
    	tostring(grab.over_window and grab.over_window.caption),
    	rw or "None",
    	data.camera.pos.x,
    	data.camera.pos.y,
    	data.camera.pos.z)
end

function w21:setup()
	self.position.relative.left = 0
	self.position.relative.top = 0
	self.position.relative.width = 256
	self.position.relative.height = 80
	self.caption = "Caption 2"
	self.name = "Caption"
	self.visible = true
	self.enabled = true
	self.minimalized = false
	self.static_children = true
	self.a = 0.7
	self.a2 = 0.7
	self.time = 0
	self.fps = 0
	widget.setup(self)
	self:update()
end

function w21:update()
	widget.update(self)
	local f_res= 60
	if ((data.counter % f_res) == 0) then
		self._time = qE.getTickCount()
		self.fps = f_res/(qE.getTickCount() - self.time)*1000
		self.time = self._time
	end	
	data.counter = data.counter + 1
	self.context = string.format("Counter: %d\nFps: %.2f\nGlyphs: %d\n",data.counter,qE.getFPS(),gl.GetGlyphsCached())
end
]]--
