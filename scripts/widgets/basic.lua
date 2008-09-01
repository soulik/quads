require('scripts/common')
require('scripts/events')
require('scripts/widgets/events')

local VERSION = "0.2a"

ALIGN = {
	none = 0,
	left = 1,
	right = 4,
	top = 8,
	bottom = 16,
}

border_width = {
	left = 2,
	top = 2,
	right = 2,
	bottom = 2,
}

grab = {
	window = {
		left = 0,
		top = 0,
		right = 0,
		bottom = 0,
		state = 0,
		window = nil,
	},
	mouse = {
		normal = {
			x = 0,
			y = 0,
		},
		click = {
			x = 0,
			y = 0,
		},
		on_window = {
			x = 0,
			y = 0,
		},
	},
	_focused_window = desktop,
	focused_window = desktop,
	_over_window = desktop,
	over_window = desktop,
}

windows = _data:new({})


widget = _data:new({
	name = "",
	caption = "Untitled",
	context = "",
	position = {
		relative = {
			left = 0,
			top = 0,
			right = 0,
			bottom = 0,
			width = 0,
			height = 0,
		},
		absolute = {
			left = 0,
			top = 0,
			right = 0,
			bottom = 0,
			width = 0,
			height = 0,
		},
	},
	align = 0,

	enabled = false,
	visible = false,
	minimalized = false,
	static_children = false,
	resizeable = true,
	movable = true,

	style = {
		title = false,
		border = false,
		caption = false,
	},

	child = {},
	parent = {},
	update = function() end,
	setup = function() end,
	event = function() end,
	draw = function() end,
	isMouseOn = function() end,
	parent_class= nil,
	events = {},
})

widget.config = {
	WM_DEBUG = true,
	EDGE = 6,
	MINI_EDGE = 6,
	min_width = 64,
	min_height = 32,
}

function widget:version()
	return "Widget system v "..VERSION
end

require(theme..'/common')

function isIn(x,y,l,t,r,b)
	if ((x >= l) and (y >= t) and (x <= r) and (y <= b)) then
   		return true
	else
		return false
	end
end

function widget:getString()
	return tostring(self.name)..", "..tostring(self.caption)
end

function widget:resetGrab()
	if (grab.window.window) then
		print ("resetGrab()")
	end
	grab.window.window = nil
	grab.window.left = 0
	grab.window.top = 0
	grab.window.right = 0
	grab.window.bottom = 0
	grab.window.state = 0
end

function widget:setGrab(wnd)
	if (grab.window.window == nil) then
		print ("setGrab()")
	end
	grab.window.window = wnd
	grab.window.left = wnd.position.relative.left
	grab.window.top = wnd.position.relative.top
	grab.window.right = wnd.position.relative.right
	grab.window.bottom = wnd.position.relative.bottom
	grab.window.state = wnd.state
end

function widget:getWidth()
	local width = self.position.relative.right - self.position.relative.left
	if (width<widget.config.min_width) then
		width = widget.config.min_width
	end
	return width
end

function widget:getHeight()
	local height = self.position.relative.bottom - self.position.relative.top
	if (height<widget.config.min_height) then
		height = widget.config.min_height
	end
	return height
end

function widget:getAbsWidth()
	local width = self.position.absolute.right - self.position.absolute.left
	return width
end

function widget:getAbsHeight()
	local height = self.position.absolute.bottom - self.position.absolute.top
	return height
end

function widget:getRight(width)
	return self.position.relative.left + self.position.relative.width
end

function widget:getBottom(height)
	return self.position.relative.top + self.position.relative.height
end

function widget:new(o,b)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	o = table.merge(self,o)
	if (b) then
		o = table.merge(o,b)
		o.parent_class = b
	end
	return o
end

function widget:newEx(a,b)
	local o = {}
	setmetatable(o, self)
	self.__index = self
	o = table.merge(self,o)
	if (b) then
		o = table.merge(o,b)
		o.parent_class = b
	end
	if (type(a)=="table") then
		-- print("AddChild(): "..tostring(a))
		a:addChild(o)
	end
	return o
end

function widget:resetWindows()
	if (type(grab.window.window)=="table") then
		grab.window.window = nil
	end
	--print("WndReset()",self.caption)
end

function widget:isMouseOn(x,y)
	if (widget.config.WM_DEBUG) then
		--print("isMouseOn",x,y)
		--print(self.position.absolute.left,self.position.absolute.top,self.position.absolute.right,self.position.absolute.bottom)
	end
	if (self.minimalized == false) then
		return isIn(x,y,self.position.absolute.left, self.position.absolute.top, self.position.absolute.right, self.position.absolute.bottom)
	else
		return isIn(x,y,self.position.absolute.left, self.position.absolute.top, self.position.absolute.right, self.position.absolute.top+window.minimalized_height)
	end
end

function widget:isMouseOnEdge(x,y)
	local _status = 0

	if (self.minimalized == false and self.static_children == false) then
		-- bottom edge
		if (isIn(x,y, self.position.absolute.left, self.position.absolute.bottom-widget.config.EDGE, self.position.absolute.right, self.position.absolute.bottom+widget.config.MINI_EDGE)) then
			_status = _status + 1
		end
		-- right edge
		if (isIn(x,y, self.position.absolute.right-widget.config.EDGE, self.position.absolute.top, self.position.absolute.right+widget.config.MINI_EDGE, self.position.absolute.bottom)) then
			_status = _status + 2
		end
		-- top edge
		if (isIn(x,y, self.position.absolute.left, self.position.absolute.top-widget.config.MINI_EDGE, self.position.absolute.right, self.position.absolute.top+widget.config.EDGE)) then
			_status = _status + 4
		end
		-- left edge
		if (isIn(x,y, self.position.absolute.left-widget.config.MINI_EDGE, self.position.absolute.top, self.position.absolute.left+widget.config.EDGE, self.position.absolute.bottom)) then
			_status = _status + 8
		end
	end
	return _status
end

function widget:update()
	if (self.enabled) then
		self:updateAbsolutePosition()
		self.pcall(self,"update",event,param1,param2)
		if (type(self.child)=="table") then
			for a,b in pairs(self.child) do
				if (type(b)=="table") then
					b:update()
				else
					print("Update() Corrupted child "..self.caption.."["..tostring(a).."]: "..tostring(b))
					_serialize2(self.child,"self.child")
				end
			end
		end
	end
	return 1
end

function widget:getAbsolutePosition()
	local temp = {}
	print("")
	if (self.static_children==true) then
		temp.left = self.position.relative.left+self.parent.position.absolute.left
		temp.top = self.position.relative.top+self.parent.position.absolute.top
		temp.right = self.position.relative.right+self.parent.position.absolute.left
		temp.bottom = self.position.relative.bottom+self.parent.position.absolute.top
	else
		temp.left = self.position.absolute.left
		temp.top = self.position.absolute.top
		temp.right = self.position.absolute.right
		temp.bottom = self.position.absolute.bottom
	end
	return temp
end

function widget:updateAbsolutePosition()
	--print("updateAbsPos",self:getString())
	local pos = {
		left = 0,
		right = 0,
		width = 0,
		height = 0,
		top = 0,
		bottom = 0,
	}
	if (self.parent and self.parent.position) then
		pos.left = self.parent.position.absolute.left
		pos.top = self.parent.position.absolute.top
		pos.right = self.parent.position.absolute.right
		pos.bottom = self.parent.position.absolute.bottom
	end
	
	self.position.absolute.left = self.position.relative.left+pos.left
	local temp = 0
		-- nastavenie sirky a vysky resp. praveho a dolneho rohu okna - podla toho, co ma okno zadefinovane
		if (self.position.relative.right ~= 0) then
			self.position.absolute.width = self:getWidth()--self.position.relative.width
			self.position.absolute.right = self.position.absolute.left+self.position.absolute.width
		else
			self.position.absolute.width = self.position.relative.width
			self.position.absolute.right = self.position.absolute.left+self.position.absolute.width
		end

		if (self.position.relative.bottom ~= 0) then
			self.position.absolute.height = self:getHeight()--self.position.relative.height
			self.position.absolute.bottom = self.position.absolute.top+self.position.absolute.height
		else
			self.position.absolute.height = self.position.relative.height
			self.position.absolute.bottom = self.position.absolute.top+self.position.absolute.height
		end
	if (self.static_children == true) then
		-- ak ma rodicovske okno titulok, posun jeho dcerske okna pod uroven titulku
		if (self.parent and self.parent.style.title==true) then
			if (Positions.caption_box_height) then
				temp = Positions.caption_box_height
			else
				temp = 24
			end
		end
		-- osestrenie pre dcerske okna, ak su vacsie ako rodicovske okno
		if (type(self.parent)=="table") then
			if (self:getAbsWidth() > self.parent:getWidth()) then
				--print("It's here")
				self.position.absolute.right = self.position.absolute.left + self.parent:getWidth()
				self.position.absolute.width = self:getAbsWidth()
			end
			if (self:getAbsHeight()+temp+1 > self.parent:getHeight()) then
				self.position.absolute.bottom = self.position.absolute.top + self.parent:getHeight()-temp-1
				self.position.absolute.height = self:getAbsHeight()
			end
		end
		-- osestrenie pre dcerske okna, ak rodicovske okno ma aktivne hrany
		if (self.parent and self.parent.style.border==true) then
			--print("Here!")
			self.position.absolute.left = self.position.absolute.left + border_width.left
			self.position.absolute.top = self.position.absolute.top + border_width.top
			self.position.absolute.right = self.position.absolute.right - border_width.right-1
			self.position.absolute.bottom = self.position.absolute.bottom - border_width.bottom-1
		end

	end

	self.position.absolute.top = self.position.relative.top+pos.top+temp+2
end

function widget:getWindowFromPos(x,y)
	if (self:isMouseOn(x,y)) then
		if (type(self.child)=="table" and self.child[1]) then
			for a,b in pairs(self.child) do
				if (type(b)=="table") then
					local ret = b:getWindowFromPos(x,y)
					if (ret) then
						return ret
					end
				else
					print("Update() Corrupted child "..self.caption.."["..tostring(a).."]: "..tostring(b))
					_serialize2(self.child,"self.child")
				end
			end
		end
		return self
	end
	return nil
end

function widget:addChild(wnd)
	table.insert(self.child, wnd)
	wnd.parent = self
end

function widget:isParent(wnd)
	if (wnd and self.parent) then
		if (self.parent == wnd) then
			return self.parent
		else
			print (tostring(self.parent))
			return self.parent:isParent(wnd)
		end
	else
		return nil
	end
end

function widget:setup()
	print("setup() "..self.caption)
	
	if (self.position.relative.width>0) then
		self.position.relative.right = self:getRight(self.position.relative.width)
	end
	
	if (self.position.relative.height>0) then
		self.position.relative.bottom = self:getBottom(self.position.relative.height)
	end

	if (self.static_children==true) then
		--self.position.relative.left = self.parent.position.absolute.left+grab._x
		--self.position.relative.top = self.parent.position.absolute.top+grab._y
	end
	self:updateAbsolutePosition()
    -- add default event handlers
    self:addEvent(events.onMouseMove,default_events.onMouseMove)
    
    self:addEvent(events.onMouseLeftDown,default_events.onMouseLeftDown)
    self:addEvent(events.onMouseLeftUp,default_events.onMouseLeftUp)
    
    self:addEvent(events.onMouseMiddleDown,default_events.onMouseMiddleDown)
    self:addEvent(events.onMouseMiddleUp,default_events.onMouseMiddleUp)

    self:addEvent(events.onMouseRightDown,default_events.onMouseRightDown)
    self:addEvent(events.onMouseRightUp,default_events.onMouseRightUp)
    
    self:addEvent(events.onKeyDown,default_events.onKeyDown)
    self:addEvent(events.onKeyUp,default_events.onKeyUp)
    self:addEvent(events.onFocus,default_events.onFocus)
    self:addEvent(events.onLostFocus,default_events.onLostFocus)
    self:addEvent(events.onClick,default_events.onClick)
    self:addEvent(events.onDoubleClick,default_events.onDoubleClick)

    self:addEvent(events.onMove,default_events.onMove)
    self:addEvent(events.onResize,default_events.onResize)


	self.pcall(self,"setup")
	if (type(self.child)=="table") then
		for a,b in pairs(self.child) do
			b:setup()
		end
	end
end

function widget:callC(event,params)
	if (self.enabled and self.visible) then
		-- call process window function
		--self.pcall(self,"event",event,param1,param2)
		if (type(self.child)=="table" and self.minimalized == false) then
			-- call event on every child
			local a
			local b
			for a,b in pairs(self.child) do
				if(b:callC(event,params) == 1) then
  					return 1
				end
			end
		end
		self:call(event,params)
	end
end

function widget:call(event, params)
    --print("call",tostring(event),tostring(params))
    if (event) then
		if (self.events[event]) then
			if (type(self.events[event])=="table") then
				for i,v in ipairs(self.events[event]) do
					v(self,event,params)	
				end
			elseif (type(self.events[event])=="function") then
				self.events[event](self,event,params)
			else
				dprintf("This is not a function ("..tostring(self.events[event])..")! Occured while calling "..tostring(event))
			end
		else
			--_serialize2(self.events,"self.events")
			dprintf("Unknown event called "..tostring(event).." on "..tostring(self.caption))
		end
	end
end

function widget:pcall(name,...)
	if (self.parent_class) then
		local f = self.parent_class[name]
		if (f) then
			if (arg) then
				--print(unpack(arg))
				--_serialize2(arg,"arg")
			end
			f(self,unpack(arg))
		end
	end
end

function widget:addEvent(event,p1,p2)
	if (event) then
		if (type(self.events) ~= "table") then
			self.events = {}
		end
		if (type(self.events[event]) ~= "table") then
			self.events[event] = {}
		end
		if (p2 == nil) then
			table.insert(self.events[event],p1)
		else
			table.insert(self.events[event],p1,p2)
		end
	end
end

function widget:draw()
	if (self.visible) then
		gl.Clear(gl_enum.GL_STENCIL_BUFFER_BIT)
		if (type(self.parent)=="table" and self.parent.cutWindow) then
			self.parent:cutWindow()
		end
		self.pcall(self,"draw")
		--self.pcall(self,"draw")
		if (self.minimalized == false) then
			for a,b in pairs(self.child) do
				if (type(b)=="table") then
					if (front_window ~= b) then
						if (b.visible) then
							b:draw()
						end
					end
				else
					print("Draw() Corrupted child "..self.caption.."["..tostring(a).."]: "..tostring(b))
					_serialize2(self.child,"self.child")
				end
			end
		end
	end
end

function widget:alignedResize(params)
	if (bAnd(self.align,ALIGN.left)) then
		--self.
	end
end

function widget:cutWindow()
	gl.ColorMask(0,0,0,0)
	gl.Disable(gl_enum.GL_DEPTH_TEST)	                      
	gl.StencilFunc(gl_enum.GL_ALWAYS,1,1)
	gl.StencilOp(gl_enum.GL_REPLACE,gl_enum.GL_REPLACE,gl_enum.GL_REPLACE)	

	gl.Begin(gl_enum.GL_QUADS)
	gl.Vertex2f (self.position.absolute.left, self.position.absolute.top)
	gl.Vertex2f (self.position.absolute.right, self.position.absolute.top)
	gl.Vertex2f (self.position.absolute.right, self.position.absolute.bottom)
	gl.Vertex2f (self.position.absolute.left, self.position.absolute.bottom)
	gl.End()

	gl.ColorMask(1,1,1,1)
	gl.StencilOp(gl_enum.GL_KEEP,gl_enum.GL_KEEP,gl_enum.GL_KEEP)	

	gl.StencilFunc(gl_enum.GL_EQUAL,1,1)
end
