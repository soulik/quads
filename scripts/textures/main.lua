require('scripts/common')
require('scripts/gl')
require('scripts/gl_common')

function tex1()
	gl.Finish()
	gl.Uniform(shaders["uniform"][0],1)
	--set orthogonal view
	prepareOrthogonal(128,128)
	gl.ClearColor(0,0,0,0.2)
	gl.Clear(bOr(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)

	img1 = tex["textures\\base_floor\\clang_floor.jpg"]
	img2 = tex["animated01"]
	img3 = tex["textures\\x\\two.jpg"]

	gl.BindTexture(gl_enum.GL_TEXTURE_2D,img1.id)
	drawBox(0,0,128,128,1,0,0,1)
	gl.BindTexture(gl_enum.GL_TEXTURE_2D,img3.id)
	drawBox(16,16,112,112,1,1,0,1)

	gl.Disable(gl_enum.GL_BLEND)

	gl.FrontFace(gl_enum.GL_CCW)
	gl.Translatef(	math.cos((data.counter/4) / 180*math.pi)*64+16,
					math.sin((data.counter/4) / 180*math.pi)*64+64-8,
					0)
	gl.Scalef(1,-1,1)

	gl.Uniform(shaders["uniform"][0],0)
	gl.printf(2,2,0xFFFFFFFFF,string.format('%u',data.counter))
	gl.Uniform(shaders["uniform"][0],1)
	  
	-- Get the texture from the framebuffer:
	gl.BindTexture(gl_enum.GL_TEXTURE_2D,img2.id)

	gl.CopyTexSubImage2D(gl_enum.GL_TEXTURE_2D, 0, 0,0,0,0,128, 128)

	leaveOrthogonal()
	gl.ClearColor(0,0,0.25,1)
	gl.Clear(bOr(gl_enum.GL_COLOR_BUFFER_BIT,gl_enum.GL_DEPTH_BUFFER_BIT))

	gl.FrontFace(gl_enum.GL_CW)
	gl.Finish()
	gl.Uniform(shaders["uniform"][0],0)
end

function tex2()
	gl.Finish()
	gl.Uniform(shaders["uniform"][0],2)
	prepareOrthogonal(64,64)

	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)

	gl.ClearColor(0,0,0,0)
	gl.Clear(bOr(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))


	drawBox2(16,16,32,32,1,0,0,0.5)
	drawBox2(24,24,16,16,0,1,0,0.5)

	gl.Disable(gl_enum.GL_BLEND)

	img = tex["animated02"]
	gl.BindTexture(gl_enum.GL_TEXTURE_2D,img.id)
	gl.CopyTexSubImage2D(gl_enum.GL_TEXTURE_2D, 0, 0,0,0,0,64, 64)

	leaveOrthogonal()
	gl.ClearColor(0,0,0.25,1)
	gl.Clear(bOr(gl_enum.GL_COLOR_BUFFER_BIT,gl_enum.GL_DEPTH_BUFFER_BIT))

	gl.FrontFace(gl_enum.GL_CW)
	gl.Finish()
	gl.Uniform(shaders["uniform"][0],0)
end