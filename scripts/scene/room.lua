require('scripts/common')
require('scripts/gl')
require('scripts/map')


room= {}

function room:prepare()
	gl.lists[5] = gl.GenLists(1)

	gl.NewList(gl.lists[5],gl_enum.GL_COMPILE);
		gl.Enable(gl_enum.GL_TEXTURE_2D)
		gl.Translatef(0,0,50)
		drawModel('room.3ds',0)
		gl.Disable(gl_enum.GL_TEXTURE_2D)
	gl.EndList()
end

function room:draw()
	gl.CallList(gl.lists[5])
	local p = data.camera.pos
	--qE.setPosition(p.x,p.y,10)
end
