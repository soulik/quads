require('scripts/common')
require('scripts/events')
require('scripts/widgets/basic')

function widget:processOnce()
    if ((isButton(1) == false) and type(over_window)=="table") then
    	edge_test = over_window:isMouseOnEdge(data.mouse.x,data.mouse.y)

    	--	1 - bottom
    	--	2 - right
    	--	4 - top
    	-- 	8 - left

    	if (edge_test ~= _edge_test) then
    		if (edge_test == 1 or edge_test == 4) then
    			setCursor("vertical")
    		elseif (edge_test == 2 or edge_test == 8) then
    			setCursor("horizontal")
    		elseif (edge_test == 3 or edge_test == 12) then
    			setCursor("diagonal1")
    		elseif (edge_test == 6 or edge_test == 9) then
    			setCursor("diagonal2")
    		else
    			setCursor("normal")
    		end
    	end
    	_edge_test = edge_test
    end

	if (grab.window.window) then
		if (isButton(1)) then
			local delta_x = (data.mouse.x - data.mouse.cx)
			local delta_y = (data.mouse.y - data.mouse.cy)
			if (edge_test == 0) then
			    if (data.mouse.cy < grab.window.top+window.minimalized_height) then
					local x = grab.window.left + delta_x 
					local y = grab.window.top + delta_y
					grab.window.window:call(events.onMove,{x,y})
				end
			else
				local left = grab.window.left
				local top = grab.window.top
				local right = grab.window.right
				local bottom = grab.window.bottom
				if (bAnd(edge_test,1) == 1) then
					bottom = bottom + delta_y
				end
				if (bAnd(edge_test,2) == 2) then
					right = right + delta_x	
				end
				if (bAnd(edge_test,4) == 4) then
					top = top + delta_y	
				end
				if (bAnd(edge_test,8) == 8) then
					left = left + delta_x	
				end
				-- now let's fix window minimal size
				if (bAnd(edge_test,1) == 1) then
					if (bottom-top < widget.config.min_height) then
						bottom = top+widget.config.min_height
					end
				end
				if (bAnd(edge_test,2) == 2) then
					if (right-left < widget.config.min_width) then
						right = left+widget.config.min_width
					end
				end
				if (bAnd(edge_test,4) == 4) then
					if (bottom-top < widget.config.min_height) then
						top = bottom-widget.config.min_height
					end
				end
				if (bAnd(edge_test,8) == 8) then
					if (right-left < widget.config.min_width) then
						left = right-widget.config.min_width
					end
				end
				grab.window.window:callC(events.onResize,{left,top,right,bottom})
			end
		end
	end


end                 	            