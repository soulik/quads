require('scripts/common')
require('scripts/events')
require('scripts/gl')
require('scripts/gl_common')
require(theme..'/utils')

window = {
	minimalized_height = 32,
	alpha = 1.0
}

function window:onMouseLeftDown(event,params)
	self:setGrab(self)
	return 1
end

function window:onDoubleClick(event,params)
	if (self.style.title==true and params[2] < self.position.absolute.top+window.minimalized_height) then
		if (self.minimalized == false) then
			self.minimalized = true
			--dprintf("Window '"..tostring(self.caption).."' minimalized")
		else
			self.minimalized = false
			--dprintf("Window '"..tostring(self.caption).."' unminimalized")
		end
	end
	return 1
end

function window:drawNormal()
	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINU_SRC_ALPHA)
	-- main window box
	if (grab.focused_window == self) then
		drawRectangle(self.position.absolute.left,self.position.absolute.top,self:getWidth(),self:getHeight(),ColorTheme.focus.window)
		if (self.style.title==true) then
			drawCaptionBox(self.position.absolute.left,self.position.absolute.top,self:getWidth(),Positions.caption_box_height,ColorTheme.focus.caption_boxL,ColorTheme.focus.caption_boxR)
		end
	else
		drawRectangle(self.position.absolute.left,self.position.absolute.top,self:getWidth(),self:getHeight(),ColorTheme.normal.window)
		if (self.style.title==true) then
			drawCaptionBox(self.position.absolute.left,self.position.absolute.top,self:getWidth(),Positions.caption_box_height,ColorTheme.normal.caption_boxL,ColorTheme.normal.caption_boxR)
		end
	end

	-- borders
	if (self.style.border==true) then
		drawWindowBorders(self.position.absolute.left,self.position.absolute.top,self:getWidth(),self:getHeight())
	end
	--drawRectangleEx(self.x,config.screen.height-self.y-self.height,self.width,-self.height,{1,0,0,1})
	if (self.style.title==true) then
		gl.Scissor(self.position.absolute.left,config.screen.height-self.position.absolute.top-self:getHeight(),self:getWidth(),self:getHeight())
		gl.Enable(gl_enum.GL_SCISSOR_TEST)
			if (self.caption) then
				gl.BlendFunc(gl_enum.GL_ONE, gl_enum.GL_ONE);
				gl.tprintf(self.position.absolute.left+8,self.position.absolute.top+2,1,1,0xFFFFFFFF,self.caption)
				gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
			end
		gl.Disable(gl_enum.GL_SCISSOR_TEST)
	end
	gl.Disable(gl_enum.GL_BLEND)
end

function window:drawMini()
	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
	-- main window box
	if (grab.focused_window == self) then
		drawRectangle(self.position.absolute.left,self.position.absolute.top,self.position.absolute.width,window.minimalized_height,ColorTheme.focus.window)
		if (self.style.title==true) then
			drawCaptionBox(self.position.absolute.left,self.position.absolute.top,self:getWidth(),Positions.caption_box_height,ColorTheme.focus.caption_boxL,ColorTheme.focus.caption_boxR)
		end
	else
		drawRectangle(self.position.absolute.left,self.position.absolute.top,self.position.absolute.width,window.minimalized_height,ColorTheme.normal.window)
		if (self.style.title==true) then
			drawCaptionBox(self.position.absolute.left,self.position.absolute.top,self:getWidth(),Positions.caption_box_height,ColorTheme.normal.caption_boxL,ColorTheme.normal.caption_boxR)
		end
	end

	-- borders
	if (self.style.border==true) then
		drawWindowBorders(self.position.absolute.left,self.position.absolute.top,self:getWidth(),window.minimalized_height)
	end

	if (self.style.title==true) then
		gl.Scissor(self.position.absolute.left,config.screen.height-self.position.absolute.top-self:getHeight(),self:getWidth(),self:getHeight())
		gl.Enable(gl_enum.GL_SCISSOR_TEST)
			if (self.caption) then
				--glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
				gl.BlendFunc(gl_enum.GL_ONE, gl_enum.GL_ONE);
				gl.tprintf(self.position.absolute.left+8,self.position.absolute.top+2,1,1,0xFFFFFFFF,self.caption);
				gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
			end
			--window.drawContext(self);
		gl.Disable(gl_enum.GL_SCISSOR_TEST)
	end
	gl.Disable(gl_enum.GL_BLEND)
end

function window:draw()
	if (self == front_window) then
		if (front_window_d==1) then
			print('Duplicite drawing')
			return nil
		else
			front_window_d=1
		end
	end
	--gl.Uniform(shaders["uniform"][0],4)
	--self.alpha = 1.0
	isError()
	if (self.minimalized == false) then
		window.drawNormal(self)
	else
		window.drawMini(self)
	end
end

function window:setup()
	return widget:setup()
end
