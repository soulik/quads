config = {};

require('scripts/common');
require('scripts/SDL');

--
--	configuration
--
	config.screen = {};
	config.screen.width = 800;
	config.screen.height = 600;
	config.screen.red_size = 8;
	config.screen.green_size = 8;
	config.screen.blue_size = 8;
	config.screen.depth_size = 16;
	config.screen.doublebuffer = 1;

	--	keyboard
	config.keyb = {}
	config.keyb.forward = SDLKey.SDLK_w
	config.keyb.backward = SDLKey.SDLK_s
	config.keyb.strafe_left = SDLKey.SDLK_a
	config.keyb.strafe_right = SDLKey.SDLK_d
	config.keyb.fly_up = SDLKey.SDLK_PAGEUP
	config.keyb.fly_down = SDLKey.SDLK_PAGEDOWN
	config.keyb.quit = SDLKey.SDLK_ESCAPE
	config.keyb.modificator = SDLMod.KMOD_SHIFT

	config.data_dir = "DATA\\"
	config.font_dir = config.data_dir.."Fonts\\"
	config.shaders_dir = "shaders\\"
	config.models_dir = config.data_dir.."models\\"
	config.texts_dir = config.data_dir.."texts\\"

	-- USED THEME
	config.gui_theme = 'quads'
	config.click_tolerance = 2
	config.double_click_time = 300
	theme = 'scripts/themes/'..config.gui_theme
	debugModel = 'players\\sarge'

-- configuration();

dprintf("Configuration loaded successfuly");