require('scripts/config')
require('scripts/common')
require('scripts/events')
require('scripts/gl')
require('scripts/gl_common')
require(theme..'/utils')
require(theme..'/window')
require(theme..'/button')
require(theme..'/caption')
require(theme..'/scrollable')

ThemeInfo = {
	name = "Quads theme",
	description = "Initial Quads GUI theme",
	author = "Soulik",
	created_on = "2.11.2007",
	widgets = {
		"window","button","scrollable","caption",
	}
}

ColorTheme = {
	normal = {
		window = {0.6,0.6,0.6,0.7},
		caption_boxL = {0.65,0.65,0.65,0.7},
		caption_boxR = {0.78,0.78,0.78,0.7},
		caption_text = {1,1,1,1},
		text = {1,1,1,1},
	},
	over = {
		window = {0.7,0.7,0.7,0.8},
		caption_boxL = {0.2,0.2,0.2,0.7},
		caption_boxR = {0.45,0.45,0.45,0.7},
		caption_text = {1,1,1,1},
		text = {1,1,1,1},
	},
	focus = {
		window = {0.7,0.7,0.7,0.8},
		caption_boxL = {0.2,0.2,0.2,0.7},
		caption_boxR = {0.45,0.45,0.45,0.7},
		caption_text = {1,1,1,1},
		text = {1,1,1,1},
	}
}

Positions = {
	caption_box_height = 24,
}