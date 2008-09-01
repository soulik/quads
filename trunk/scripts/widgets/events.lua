require('scripts/common')
require('scripts/events')

default_events = {}

function default_events:onMouseLeftDown(event,params)
	print("onMouseLeftDown() "..self.caption)
	self.pcall(self,"onMouseLeftDown",event,params)
	-- where did we click (window area)
	grab.mouse.on_window.x = self.position.relative.left-data.mouse.x
	grab.mouse.on_window.y = self.position.relative.top-data.mouse.y
	-- old window position
	grab.window.x = self.position.relative.left
	grab.window.y = self.position.relative.top
	if (self ~= grab.focused_window) then
		self:call(events.onFocus,{})	
	end
	return 1
end

function default_events:onClick(event,params)
	if ((qE.getTickCount()-dclick_time)<config.double_click_time) then
		self:call(events.onDoubleClick,params)
	else
		dclick_time = qE.getTickCount()
		self.pcall(self,"onClick",event,params)
		print("onClick() "..self.caption)
	end
	
	if (focused_window ~= self) then
		widget.event(focused_window,events.onLostFocus, params)
		widget.event(self,events.onFocus, params)
		focused_window = self
	end
	return 1
end

function default_events:onDoubleClick(event,params)
	dclick_time = 0
	print("onDoubleClick() ",self.caption,params)
	self.pcall(self,"onDoubleClick",event,params)
	return 1
end

function default_events:onMouseLeftUp(event,params)
	print("onMouseLeftUp() "..self.caption)
	self.pcall(self,"onMouseLeftUp",event,params)
	if (self == grab.over_window and self == grab.focused_window) then
		self:call(events.onClick,params)
	end
	return 1
end

function default_events:onMouseMiddleUp(event,params)
	return 1
end

function default_events:onMouseMiddleDown(event,params)
	return 1
end

function default_events:onMouseRightUp(event,params)
	return 1
end

function default_events:onMouseRightDown(event,params)
	return 1
end

function default_events:onFocus(event,params)
	--print("onFocus() "..self.caption)
	self.pcall(self,"onFocus",event,params)
	grab._focused_window = grab.focused_window
	grab._focused_window:call(events.onLostFocus,params) 
	grab.focused_window = self
	front_window = self
	--move window to top of the children table on parent
	if (self.parent) then
		if (self.parent.child) then
			table.settop(self.parent.child,self)
		end
	end
	return 1
end

function default_events:onLostFocus(event,params)
	self.pcall(self,"onLostFocus",event,params)
	--print("onLostFocus() "..self.caption)
	return 1
end

function default_events:onKeyDown(event,params)
	self.pcall(self,"onKeyDown",event,params)
	--print("onKeyDown() "..self.caption)
	return 1
end

function default_events:onKeyUp(event,params)
	self.pcall(self,"onKeyUp",event,params)
	--print("onKeyUp() "..self.caption)
	return 1
end

function default_events:onMove(event,params)
	if (self.movable == false) then
		return 0
	end

	local wlimit = 32
	self.pcall(self,"onMove",event,params)
	self.position.relative.left = params[1]
	self.position.relative.top = params[2]
	self.position.relative.right = self.position.relative.left+self.position.relative.width
	self.position.relative.bottom = self.position.relative.top+self.position.relative.height
	self:updateAbsolutePosition()
	return 1
end

function default_events:onResize(event,params)
	if (self.resizeable == false) then
		return 0
	end
	--print("onResize ",tostring(self.caption))
	self.pcall(self,"onResize",event,params)

	if (self.static_children ~= true) then
		local wlimit = 32
		self.position.relative.left = params[1]
		self.position.relative.top = params[2]
		self.position.relative.right = params[3]
		self.position.relative.bottom = params[4]
		self.position.relative.width = self:getWidth()
		self.position.relative.height = self:getHeight()
		self:updateAbsolutePosition()
	else
	end
	return 1
end

function default_events:onMouseMove(event,params)
	over_window = self
	self.pcall(self,"onMouseMove",event,params)
	--print("onMouseMove() "..self.caption)
	return 1
end
