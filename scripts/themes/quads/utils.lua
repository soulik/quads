require('scripts/config')
require('scripts/common')
require('scripts/events')
require('scripts/gl')
require('scripts/gl_common')

function drawRectangleEx(x,y,width,height,color)
	local _y = config.screen.height-y
	gl.Begin(gl_enum.GL_LINE_STRIP)
		gl.Color4f(color[1],color[2],color[3],color[4])
		gl.Vertex2f(x,_y)
		gl.Vertex2f(x+width,_y)
		gl.Vertex2f(x+width,_y+height)
		gl.Vertex2f(x,_y+height)
		gl.Vertex2f(x,_y)
	gl.End()
end

function drawWindowBorders(x,y,width,height)
	gl.Begin(gl_enum.GL_LINE_STRIP)
		gl.Color4f(0.9, 0.9, 0.9, 0.7)
		gl.Vertex2f(x, y)
		gl.Color4f(0.5, 0.5, 0.5, 0.7)
		gl.Vertex2f(x+width, y)
		gl.Color4f(0.1, 0.1, 0.1, 0.7)
		gl.Vertex2f(x+width, y+height)
		gl.Color4f(0.5, 0.5, 0.5, 0.7)
		gl.Vertex2f(x, y+height)
		gl.Color4f(0.9, 0.9, 0.9, 0.7)
		gl.Vertex2f(x, y)
	gl.End()
end

function drawWindowBox(x,y,width,height)
	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(1,1,1,0.7)
		gl.TexCoord2f(0,0)
		gl.Vertex2f(x,y)
		gl.TexCoord2f(1,0)
		gl.Vertex2f(x+width,y)
		gl.Color4f(1,1,1,1.0)
		gl.TexCoord2f(1,1)
		gl.Vertex2f(x+width,y+height)
		gl.TexCoord2f(0,1)
		gl.Vertex2f(x,y+height)
	gl.End()
end

function drawCaptionBox(x,y,width,height,c1,c2)
	gl.Begin(gl_enum.GL_QUADS);
			gl.Color4f(c1[1],c1[2],c1[3],c1[4]);
			gl.Vertex2f(x+2,y+2);
			gl.Color4f(c2[1],c2[2],c2[3],c2[4]);
			gl.Vertex2f(x+width-2,y+2);
			gl.Vertex2f(x+width-2,y+height+2);
			gl.Color4f(c1[1],c1[2],c1[3],c1[4]);
			gl.Vertex2f(x+2,y+height+2);
	gl.End();

end

function draw_debug()
	-- gl.Uniform(shaders["uniform"][0],3)
	if (data.show_gui and data.mouse.button>0) then
	gl.PrepareOrthogonal()
	x = 320
	y = 16
	width = 64
	height = 64
	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)
	gl.LineWidth(3)
	gl.Begin(gl_enum.GL_LINES)
		gl.Color4f(0,1,0,0.7)
		gl.Vertex2f(data.mouse.cx,data.mouse.cy)
		gl.Color4f(1,0.2,0,0.7)
		gl.Vertex2f(data.mouse.x,data.mouse.y)
	gl.End()
	gl.LineWidth(1)

	gl.Disable(gl_enum.GL_BLEND)
	end
end

