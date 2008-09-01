require ('scripts/config')
require ('scripts/common')
require ('scripts/gl')
require ('scripts/SDL')

data.grab = false

function setMouseButton(button,button2)
	data.mouse.last_button = bXor(button,data.mouse.button)	
	data.mouse.button = button
	data.mouse.button2 = button2
end

function setMousePos(x,y,rx,ry)
	data.mouse.x = x
	data.mouse.y = y
	data.mouse.rx = rx
	data.mouse.ry = ry
end

function BUTTON(btn)
	return bShl(1,btn-1)
end

function isButton(btn)
	local b = bAnd(data.mouse.button,BUTTON(btn))
	--print (btn, b)
	if (b>0) then
		return true
	else
		return false
	end
end

function processMouse()
	if (bAnd(data.keyMod,SDLMod.KMOD_SHIFT) == 0) then
		data.mouse.click = false
		if (data.mouse.button ~= data.mouse._button )  then
			-- grab object from 3d space
			--data.grab = true
			data.mouse.click = true
			data.mouse.cx = data.mouse.x
			data.mouse.cy = data.mouse.y
			data.mouse._button = data.mouse.button
		else
			--data.grab = false
		end
	else
		local button = qE.getMouseButton()
		if (button>0) then
			--print(button)
		end
		--qE.setAngleFromRelMouse(data.mouse.rx,data.mouse.ry)
		--qE.lookAt(data.camera.pos.x,data.camera.pos.y,data.camera.pos.z,data.camera.angle.x+data.mouse.ry,0,data.camera.angle.z+data.mouse.rx,50)
		--qE.lookAt(models[debugModel].pos.x,models[debugModel].pos.y,models[debugModel].pos.z,data.camera.angle.x+data.mouse.ry,0,data.camera.angle.z+data.mouse.rx,data.camera.distance)
		--qE.setAngle(data.camera.angle.x+data.mouse.ry,data.camera.angle.y,data.camera.angle.z+data.mouse.rx)
	end
	if (data.mouse.button>0) then
		if (isButton(5)) then
			if (data.camera.distance<500) then
				data.camera.distance = data.camera.distance + 1.2
			end
		end
		if (isButton(4)) then
			if (data.camera.distance>30) then
				data.camera.distance = data.camera.distance - 1.2
	    	end
	   	end	
	else
		data.mouse.cx = 0
		data.mouse.cy = 0	
	end
	--if (data.mouse.rx>0.1) then
	--	data.mouse.rx = data.mouse.rx * 0.5
	--else
	--	data.mouse.rx = 0
	--end
	--if (data.mouse.ry>0.1) then
	--	data.mouse.ry = data.mouse.ry * 0.5
	--else
	--	data.mouse.ry = 0
	--end
	--data.mouse.rx = 0
	--data.mouse.ry = 0
end

function loadCursor(name,width,height,hotx,hoty,cdata)
	data.mouse.cursors[name] = qE.loadCursor(cdata,width,height,hotx,hoty)

end

function setCursor(name)
	qE.setCursor(data.mouse.cursors[name])
end