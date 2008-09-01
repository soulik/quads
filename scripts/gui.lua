require('scripts/common')
require('scripts/events')
require('scripts/widgets/basic')
require('scripts/widgets/evoker')
require('scripts/widgets/processor')
require('scripts/gl')
require('scripts/scene')
require('scripts/keyboard')
require('scripts/mouse')

require(theme..'/common')

dprintf(tostring(widget.version()))
dprintf("Using theme '"..ThemeInfo.name.."' by "..ThemeInfo.author)

desktop = widget:new()
            
grab.focused_window = desktop
grab.over_window = desktop

front_window = 0
front_window_d = 0
edge_test = 0
_edge_test = 0
dclick_time = 0

function desktop:setup()
	self.caption = "Desktop"
	self.name = "Desktop"
	self.position.relative.left = 0
	self.position.relative.top = 0
	self.position.relative.right = config.screen.width
	self.position.relative.bottom = config.screen.height
	self.visible = true
	self.enabled = true
	self.movable = false
	self.minimalized = false
	self.resizeable = false
	self.parent = {}
	widget.setup(self)
end

function desktop:update()
	widget.update(self)
end

function desktop:draw()
	widget.draw(self)
	draw_debug()
end

function drawWindows()

	front_window_d=0
	data.counter = data.counter + 1
	
	if (data.show_gui) then
		gl.PrepareOrthogonal()
		gl.Enable(gl_enum.GL_STENCIL_TEST)
		desktop:draw()
		if (type(front_window)=="table") then
			front_window:draw()
		end
		gl.Disable(gl_enum.GL_STENCIL_TEST)
		gl.Enable(gl_enum.GL_DEPTH_TEST)
	end
end

function updateWindows()
	desktop:update()
end

function processWindowEvents()
	processKeys()
	processMouse()
	local event = events.none
	local param1 = data.mouse.x
	local param2 = data.mouse.y
	grab._over_window = grab.over_window
	grab.over_window = desktop:getWindowFromPos(data.mouse.x,data.mouse.y)
	widget.process(desktop)
	if (over_window~=desktop) then 
		data.outer_grab = false
	else
		data.outer_grab = true
		scene.outerEvent()
	end
	widget:processOnce()
	--desktop:event(event,param1,param2)
	data.mouse.rx = 0
	data.mouse.ry = 0
end

function prepareWindows()
	table.insert(windows, desktop)
	for a,b in pairs(windows) do
		b:setup()
	end
	if (windows[1]) then
		windows[1]:call(events.onFocus,{})
	end
end
