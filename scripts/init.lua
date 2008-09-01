require('scripts/config')
require('scripts/SDL')
require('scripts/map')
require('scripts/gl')
require('scripts/debug')
require('scripts/gui')
require('scripts/windows')
require('scripts/textures')
require('scripts/terrain')
require('scripts/scene')
require('scripts/startup')
require('scripts/keyboard')
require('scripts/mouse')
require('scripts/cursors')

--
--	init
--
function init()
	dprintf('Lua Init Script')
	dprintf('Preparing windows')
	prepareWindows()
	qE.tuserdata({a="ahoj"})
end


--
--	run startup
--
function startup()
	prepareCursors()
end

--
--	init game
--
function init_game()
	dprintf("init_game()")

--	qE.generateMapV2(50,50)
--	qE.saveMapV2('map1.map')
--	initMap()
--	qE.loadTestMap(100,100)
end

--
--	init renderer
--


function init_renderer()
	qE.initData(config)
	qE.initRenderer(config.screen)
	qE.initKeyboard(config.keyb)
end

--
--	main
--
init();
