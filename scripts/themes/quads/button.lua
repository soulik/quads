require('scripts/common')
require('scripts/events')
require('scripts/gl')
require('scripts/gl_common')
require(theme..'/utils')

button = {}

function button:draw()
	--local pos = self:getAbsolutePosition()

	drawRectangle(self.position.absolute.left,self.position.absolute.top,self:getWidth(),self:getHeight(),{self.a,self.a,self.a,self.a})
	gl.BlendFunc(gl_enum.GL_ZERO, gl_enum.GL_ONE_MINUS_SRC_COLOR);
	--gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
	--gl.printf(self.x+2,self.y+2+4,0x000000FF,self.caption)
	--gl.tprintf(self.x+2,self.y+2,1,1,0xFFFFFFff,self.caption)
	gl.Scissor(self.position.absolute.left,config.screen.height-self.position.absolute.top-self:getHeight(),self:getWidth(),self:getHeight())
	gl.Enable(gl_enum.GL_SCISSOR_TEST)
		if (self.caption) then
			gl.tprintfEx	(self.position.absolute.left+2,	self.position.absolute.top+2,	0xFFFFFFFF,self.caption);
		end
		--window.drawContext(self);
	gl.Disable(gl_enum.GL_SCISSOR_TEST)

	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
end
