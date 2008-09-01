require('scripts/common')
require('scripts/gl')

function gl.ShowInfo(name)
	text = gl.GetString(name)
	error_n = gl.GetError()
	if (error_n~=gl_enum.GL_NO_ERROR) then
		dprintf("Error: %d",error_n)
		dprintf("OpenGL Error: %s",gl.ErrorString(error_n))
	end
	text = text or ""
	return text
end
function load_shader(name)
	assert(name,"Shader name is not set!")
	pixel_src = open_file(config.shaders_dir..name.."\\fragment.shader") or nil
	vertex_src = open_file(config.shaders_dir..name.."\\vertex.shader") or nil
	if (pixel_src) then
		gl.ShaderSource(shaders["pixel"], 1, pixel_src)
		gl.CompileShader(shaders["pixel"])
		gl.AttachObject(shaders["program"], shaders["pixel"])
	end
	if (vertex_src) then
		gl.ShaderSource(shaders["vertex"], 1, vertex_src)
		gl.CompileShader(shaders["vertex"])
		gl.AttachObject(shaders["program"], shaders["vertex"])
	end
	gl.LinkProgram(shaders["program"])

end

function isError()
	errn = gl.GetError()
	if (errn ~= gl_enum.GL_NO_ERROR) then
		print(">> "..tostring(gl.ErrorString(errn)).." ("..tostring(errn)..")")
	end
end

function prepareOrthogonal(width,height)
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
	gl.Viewport(0,0,width,height)
	gl.Ortho( 0,width, height,0,-1,1)
	gl.MatrixMode(gl_enum.GL_MODELVIEW)
	gl.LoadIdentity()
	
end

function leaveOrthogonal()
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
	gl.Viewport(0,0,config.screen.width,config.screen.height)
	gl.Ortho( 0, config.screen.width, config.screen.height,0,-1,1)
	gl.MatrixMode(gl_enum.GL_MODELVIEW)
	gl.LoadIdentity()
end

function drawPoint(x,y,z,size,r,g,b,a)
	if (gl.draw_points) then
		gl.PushMatrix()

		gl.LoadIdentity()
		gl.Translatef(x,y,z)
	
		gl.Rotatef(data.camera.angle.z,0,0,1)
		gl.Rotatef(90-data.camera.angle.x,1,0,0)
	
		gl.Translatef(-size/2,0,-size/2)
	
		img1 = tex["gfx\\misc\\flare.jpg"]
		gl.Disable(gl_enum.GL_DEPTH_TEST)
		gl.Enable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)
		gl.BindTexture(gl_enum.GL_TEXTURE_2D,img1.id)
		gl.Enable(gl_enum.GL_TEXTURE_2D)
		gl.Begin(gl_enum.GL_QUADS)
			
			gl.Color4f(r,g,b,a)
			gl.TexCoord2f(0,0)
			gl.Vertex3f(0,0,0)
	
			gl.Color4f(r,g,b,a)
			gl.TexCoord2f(1,0)
			gl.Vertex3f(size,0,0)
	
			gl.Color4f(r,g,b,a)
			gl.TexCoord2f(1,1)
			gl.Vertex3f(size,0,size)
	
			gl.Color4f(r,g,b,a)
			gl.TexCoord2f(0,1)
			gl.Vertex3f(0,0,size)
		gl.End()
		gl.Disable(gl_enum.GL_TEXTURE_2D)
		gl.Disable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)
		gl.Enable(gl_enum.GL_DEPTH_TEST)
		gl.PopMatrix()
	end
end

function drawBox(x,y,width,height,r,g,b,a)
	gl.Enable(gl_enum.GL_TEXTURE_2D)
	gl.Begin(gl_enum.GL_QUADS)
		
		gl.Color4f(r,g,b,a)
		gl.TexCoord2f(0,0)
		gl.Vertex2i(x,y)

		gl.Color4f(r,g,b,a)
		gl.TexCoord2f(1,0)
		gl.Vertex2i(width+x,y)

		gl.Color4f(r,g,b,a)
		gl.TexCoord2f(1,1)
		gl.Vertex2i(width+x,height+y)

		gl.Color4f(r,g,b,a)
		gl.TexCoord2f(0,1)
		gl.Vertex2i(x,height+y)
	gl.End()
	gl.Disable(gl_enum.GL_TEXTURE_2D)
end

function drawBox2(x,y,width,height,r,g,b,a)
	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(r,g,b,a)
		gl.Vertex2i(x,y)

		gl.Color4f(r,g,b,a)
		gl.Vertex2i(width+x,y)

		gl.Color4f(r,g,b,a)
		gl.Vertex2i(width+x,height+y)

		gl.Color4f(r,g,b,a)
		gl.Vertex2i(x,height+y)
	gl.End()
end


function postProcess()
	if (gl.abilities.NPOTStextures) then
		function getX(offset,dir)
			return math.cos(dir*(data.counter/20)%360+offset)*80
		end

		function getY(offset,dir)
			return math.sin(dir*(data.counter/20)%360+offset)*80
		end

		img = tex["postProcess"]
		gl.PushMatrix()
		prepareOrthogonal(config.screen.width,config.screen.height)
		isError()
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)
		gl.FrontFace(gl_enum.GL_CCW)

		gl.Uniform(shaders["uniform"][0],42)
		gl.BindTexture(gl_enum.GL_TEXTURE_2D,img.id)
		gl.CopyTexSubImage2D(gl_enum.GL_TEXTURE_2D, 0, 0,0,0,0,config.screen.width, config.screen.height)

		gl.Enable(gl_enum.GL_BLEND)

		drawBox(getX(0,1),config.screen.height+getY(0,1),config.screen.width,-config.screen.height,1,0,0,0.6)
		drawBox(getX(90,-1),config.screen.height+getY(90,-1),config.screen.width,-config.screen.height,0,1,0,0.6)
		drawBox(getX(90,2),config.screen.height+getY(90,2),config.screen.width,-config.screen.height,0,0,1,0.6)
		--drawBox(getX(0,1),config.screen.height+getX(0,1),config.screen.width,-config.screen.height,1,1,1,0.6)

		--drawBox(0,config.screen.height+offset,config.screen.width,-config.screen.height,1,1,1,0.6)
		--drawBox(0,config.screen.height-offset,config.screen.width,-config.screen.height,1,1,1,0.6)

		gl.Disable(gl_enum.GL_BLEND)
		gl.FrontFace(gl_enum.GL_CW)

		leaveOrthogonal()
		gl.Finish()
		gl.PopMatrix()
		gl.Uniform(shaders["uniform"][0],0)
	end
end

function drawRectangle(x,y,width,height,color)
	gl.Begin(gl_enum.GL_QUADS)
		--print('re')
		gl.Color4f(color[1],color[2],color[3],color[4])
		--gl.Color4f(0.7,0.7,0.7,0.7)
		gl.Vertex2f(x,y)
		gl.Vertex2f(x+width,y)
		gl.Vertex2f(x+width,y+height)
		gl.Vertex2f(x,y+height)
	gl.End()
end

function drawBox(x,y,width,height,r,g,b,a)
	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(r,g,b,a)
		gl.TexCoord2f(0,0)
		gl.Vertex2f(x,y)
		gl.TexCoord2f(1,0)
		gl.Vertex2f(x+width,y)
		gl.TexCoord2f(1,1)
		gl.Vertex2f(x+width,y+height)
		gl.TexCoord2f(0,1)
		gl.Vertex2f(x,y+height)
	gl.End()
end

function prepareScene()
	gl.Clear(bOr(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))
	--gl.Clear(gl_enum.GL_DEPTH_BUFFER_BIT)
	updateTextures()
end