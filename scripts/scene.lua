require('scripts/common')
require('scripts/gl')
require('scripts/terrain')
require('scripts/models')
require('scripts/scene/room')

scene={}

function drawAxis()
	gl.Begin(gl_enum.GL_LINES)
		gl.Color3f(1,0,0)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(10,0,0)

		gl.Color3f(0,1,0)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(0,10,0)

		gl.Color3f(0,0,1)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(0,0,10)
	gl.End()
end

function pickedObject(num)
	if (num==1) then
		w3.visible = true
		w3.enabled = true
		w32.visible = true
	end
	print ("Object: "..tostring(num).." picked")
end

function scene:draw()
	drawAxis()
	drawAlphaTest()
end

function scene:draw2()
	drawAxis()
end

function scene:prepare()
end

function scene:outerEvent()
end

function drawAlphaTest()
	local size = math.abs(math.sin((data.counter/100)%360)*0.8)+0.2
	local img = {}
	img[1] = tex['textures\\x\\diery.png']
	img[2] = tex['textures\\base_floor\\techfloor2.tga']
	img[3] = tex['textures\\base_wall\\basewall01bit.tga']
	img[4] = tex['textures\\base_floor\\techfloor.tga']
	prepareOrthogonal(config.screen.width,config.screen.height)
	
	gl.Enable(gl_enum.GL_TEXTURE_2D)
	gl.Enable(gl_enum.GL_BLEND)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE_MINUS_SRC_ALPHA)

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, img[2].id)
	gl.PushMatrix()
	gl.Translatef(64+4,64+4,0)
	gl.Scalef(size,size,0)
	gl.Rotatef(-((data.counter*2)%360),0,0,1)
	gl.Translatef(-64-4,-64-4,0)
	drawBox(4,4,128,128,1,1,1,1)
	gl.PopMatrix()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, img[2].id)
	gl.PushMatrix()
	gl.Translatef(64+4,64+4,0)
	gl.Rotatef(data.counter%360,0,0,1)
	gl.Translatef(-64-4,-64-4,0)
	drawBox(4,4,128,128,1,1,1,1)
	gl.PopMatrix()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, img[4].id)
	drawBox(4,4,128,128,1,1,1,1)

	gl.BlendFunc(gl_enum.GL_SRC_ALPHA,gl_enum.GL_ONE)
	gl.Disable(gl_enum.GL_BLEND)
	gl.Disable(gl_enum.GL_TEXTURE_2D)
	leaveOrthogonal()
	isError()
end