require ('scripts/config')
require ('scripts/common')
require ('scripts/SDL')
require ('scripts/player')

data.keyBind = {
	[0] = {
		[config.keyb.quit] = qE.quit,
		[SDLKey.SDLK_KP_PLUS] = function()
			data.models.n = data.models.n + 1
		end,
		[SDLKey.SDLK_KP_MINUS] = function()
			if (data.models.n>0) then
				data.models.n = data.models.n - 1
			end
		end
	},
	[SDLMod.KMOD_SHIFT] = {
		[config.keyb.forward] = player.moveForward,
		[config.keyb.backward] = player.moveBackward,
		[config.keyb.strafe_left] = player.strafeLeft,
		[config.keyb.strafe_right] = player.strafeRight,
		[SDLKey.SDLK_g] = player.gesture,
		[config.keyb.fly_up] = qE.flyUp,
		[config.keyb.fly_down] = qE.flyDown,
		[SDLKey.SDLK_q] = player.shot,
	}
}


function isPressed(key)
	-- little fix
	if (data.keyboard[key] and data.keyboard[key]>0) then

		return true
	else
		return false
   	end
end

function processKeys()
	data.keyMod = qE.getKeymod()
	data.key_already_processed = false
	player.moving = false
	player.gesture = false
	player.shot = false
	local a,b,c,d
	for a,b in pairs(data.keyBind) do
		if ((bAnd(data.keyMod,a)>0) or (a == 0)) then
			for c,d in pairs(b) do
				if (isPressed(c)) then
					d()
					data.key_already_processed = true
				end
			end
		end
	end
	if (bAnd(data.keyMod,SDLMod.KMOD_ALT) > 0) then
		data.camera.rotate = true
	else
		data.camera.rotate = false
	end
	if (bAnd(data.keyMod,SDLMod.KMOD_SHIFT)>0) then
		data.show_gui = false
	else         
	    data.show_gui = true
	end
end

function setKey(key,stat)
	data.keyboard[key] = stat
	data.last_key = key
	data.last_key_state = stat
end
