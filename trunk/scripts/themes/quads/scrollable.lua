require('scripts/common')
require('scripts/events')
require('scripts/SDL')
require('scripts/gl')
require('scripts/gl_common')
require(theme..'/utils')

scrollable = {
	scroll = {
		x = 0,
		y = 0,
		stepping = 5,
	}
}

function scrollable:onKeyDown(event,param1,param2)
	if (event == events.onKeyDown) then
		if (param1 == SDLKey.SDLK_UP) then
			scrollable.scrollUp(self)
		elseif (param1 == SDLKey.SDLK_DOWN) then
			scrollable.scrollDown(self)
		elseif (param1 == SDLKey.SDLK_LEFT) then
			scrollable.scrollLeft(self)
		elseif (param1 == SDLKey.SDLK_RIGHT) then
			scrollable.scrollRight(self)
		end
	end
	return 1
end

function scrollable:scrollUp()
	self.scroll.y = self.scroll.y + self.scroll.stepping
	--print("ScrollUp ("..tostring(self.scroll.x)..","..tostring(self.scroll.y)..")")
end

function scrollable:scrollDown()
	self.scroll.y = self.scroll.y - self.scroll.stepping
	--print("ScrollDown ("..tostring(self.scroll.x)..","..tostring(self.scroll.y)..")")
end

function scrollable:scrollLeft()
	self.scroll.x = self.scroll.x + self.scroll.stepping
	--print("ScrollLeft ("..tostring(self.scroll.x)..","..tostring(self.scroll.y)..")")
end

function scrollable:scrollRight()
	self.scroll.x = self.scroll.x - self.scroll.stepping
	--print("ScrollRight ("..tostring(self.scroll.x)..","..tostring(self.scroll.y)..")")
end

function scrollable:onKeyUp(event,param1,param2)
	return 1
end


function scrollable:drawNormal()
	gl.Enable(gl_enum.GL_BLEND)
	drawRectangle(self.x,self.y,self.width,self.height,{0.7,0.7,0.7,0.9 * alpha})
	gl.Scissor(self.x,config.screen.height-self.y-self.height,self.width,self.height)
	gl.Enable(gl_enum.GL_SCISSOR_TEST)
	--gl.Disable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_ZERO, gl_enum.GL_ONE_MINUS_SRC_COLOR);
		gl.PushMatrix()
		--gl.SetFontStyle(2)
		--gl.tglyph(self.x+8,self.y+8,32,32,1.2,0xFFFFFFFF,0x0069)
		gl.Translatef(self.scroll.x,self.scroll.y,0)

		gl.tprintfEx(self.x+2,	self.y+2,0xFFFFFFB0,self.context)
		--gl.SetFontStyle(0)
		--gl.BlendFunc(gl_enum.GL_ONE,gl_enum.GL_ONE)
		gl.PopMatrix()
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
	gl.Disable(gl_enum.GL_SCISSOR_TEST)
end

function scrollable:draw()
	if (self == front_window) then
		if (front_window_d==1) then
			print('Duplicite drawing')
			return nil
		else
			front_window_d=1
		end
	end
	alpha = 1.0

	isError()
	--if (self.minimalized == false) then
		scrollable.drawNormal(self)
	--else
	--	scrollable:drawMini()
	--end
end
