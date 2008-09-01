require('scripts/common')
require('scripts/events')
require('scripts/widgets/basic')

function widget:process()
	if (self.visible == false or self.enabled == false) then
		return 0
	end
	local event = events.none
	local params = {}
		
	-- skip desktop for event processing
	if (true) then --self.name ~= "Desktop") then
			-- process only if mouse cursor is on the window
			if (self == grab.over_window) then
				--print("onWindow",tostring(self.caption))
				if (grab.over_window ~= grab.over_window) then
					event,params = events.onHover,{data.mouse.x,data.mouse.y}
				end

				-- on mouse move
				--print(data.mouse.rx,data.mouse.ry)
				if (data.mouse.rx ~= 0 or data.mouse.ry ~= 0) then
					event,params = events.onMouseMove,{data.mouse.x,data.mouse.y}
				end                                      
				
				if (data.mouse.click) then
					params = {data.mouse.x,data.mouse.y}
					if (data.mouse.button == 0) then
						if (data.mouse.last_button == 1) then
							event = events.onMouseLeftUp
							self:resetGrab()
    					elseif (data.mouse.last_button == 2) then
							event = events.onMouseMiddleUp
						elseif (data.mouse.last_button == 4) then
							event = events.onMouseRightUp
						end
						--self:resetWindows()
					else
						if (data.mouse.last_button == 1) then
							event = events.onMouseLeftDown
						elseif (data.mouse.last_button == 2) then
							event = events.onMouseMiddleDown
						elseif (data.mouse.last_button == 4) then
							event = events.onMouseRightDown
						end
					end
				end

				if (math.abs(data.mouse.rx) < config.click_tolerance and math.abs(data.mouse.ry) < config.click_tolerance) then
					-- on mouse click
				end
			end
			-- let's process keyboard events if focused
			if (grab.focused_window and self == grab.focused_window) then
				-- on key pressed or released
				if (data.last_key ~= nil and data.last_key_state ~= nil) then
					if (data.key_already_processed == false) then
						params = {data.last_key}
						if (data.last_key_state == 1) then
							event = events.onKeyDown
						else
							event = events.onKeyUp
						end
					end
					data.last_key = nil
					data.last_key_state = nil
				end
			end

	end

 	if (self.enabled and self.visible) then
		-- call process window function
		--self.pcall(self,"event",event,param1,param2)
		if (type(self.child)=="table" and self.minimalized == false) then
			-- call event on every child
			local a
			local b
			for a,b in pairs(self.child) do
			    --print(tostring(b.caption),tostring(event))
				if(b:process() == 1) then
  					return 1
				end
			end
		end
	end
	
	if (widget.config.WM_DEBUG and event) then
		--print("event:",tostring(event))
	end
	if (event~=nil) then
		self:call(event,params)
	    --print(tostring(self.caption),tostring(event))
	
	end
	return 0
end
