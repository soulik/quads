require('scripts/common')
require('scripts/events')
require('scripts/SDL')
require('scripts/gl')
require('scripts/gl_common')
require(theme..'/utils')

caption = {
	scroll = {
		x = 0,
		y = 0,
		stepping = 5,
	}
}

function caption:drawContext()
	gl.BlendFunc(gl_enum.GL_ONE, gl_enum.GL_ONE);
	gl.tprintfEx(self.position.absolute.left+2,	self.position.absolute.top+2+24+4-24,	0xFFFFFFFF,self.context);
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
end

function caption:drawNormal()
	gl.Scissor(self.position.absolute.left,config.screen.height-self.position.absolute.top-self:getHeight(),self:getWidth(),self:getHeight())
	gl.Enable(gl_enum.GL_SCISSOR_TEST)
	gl.Enable(gl_enum.GL_BLEND)
	drawRectangle(self.position.absolute.left,self.position.absolute.top,self:getWidth(),self:getHeight(),{0.3,0,0.3,0.1})
	
	self:drawContext()
	--drawRectangleEx(self.position.absolute.left+1,self.position.absolute.top+1,self.position.absolute.width,self.position.absolute.height,{1,0,0,1})
	gl.Disable(gl_enum.GL_BLEND)
	gl.Disable(gl_enum.GL_SCISSOR_TEST)
end

function caption:draw()
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
		caption.drawNormal(self)
	--else
	--	caption:drawMini()
	--end
end

function caption:onResize(event,params)
    --print("caption:onResize ",tostring(self.caption))
end

function caption:setup()
    print("caption:setup ",tostring(self.caption))
	self:addEvent(events.onResize,caption.onResize)
	--return widget.setup(self)
end