require('scripts/gl');
require('scripts/textures');
require('scripts/scene');
require('scripts/models');
require('scripts/proxy')
require('scripts/windows')
require('scripts/mouse')

function renderer_ready()
	prepareRenderer()
	--gl.LoadFont(config.font_dir..'DejaVuSans.ttf',16)
	gl.LoadFont(config.font_dir..'Cyberbit.ttf',16)
	prepareTextures()
	isError()
	startup()
	setCursor("normal")
end

function prepareTextures()
	qLuaMem.Init()
	testProxy()

	loadTex('textures\\base_floor\\clang_floor.jpg',0)
	loadTex('bigchars.tga',0)
	loadTex('textures\\base_wall\\metalfloor_wall_11.jpg',1)
	loadTex('gfx\\misc\\flare.jpg',0)
	loadTex('textures\\x\\one.jpg',0)
	loadTex('textures\\x\\two.jpg',0)
	loadTex('textures\\x\\three.jpg',0)
	loadTex('textures\\x\\four.jpg',0)
	loadTex('textures\\x\\nokia_6100.jpg',0)
	--loadTex('textures\\x\\diery.png',0)
	loadTex('textures\\base_floor\\techfloor.tga',0)
	loadTex('textures\\base_floor\\techfloor2.tga',0)
	loadTex('textures\\base_wall\\basewall01bit.tga',0)
	qE.createTexture(128,128,3,"animated01")
	qE.createTexture(64,64,3,"animated02")
	addTex("animated01")
	addTex("animated02")
	if (gl.abilities.NPOTStextures) then
		qE.createTexture(config.screen.width,config.screen.height,3,"postProcess")
		addTex("postProcess")
	end
	qE.setFontTexture(tex['bigchars.tga'].id)
	scene:prepare()
	qE.setSpeed(0.1)
	-- _serialize2(w3,"w3")
end
