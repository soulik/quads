require ('scripts/config')
require ('scripts/common')
require ('scripts/SDL')
require ('scripts/models')

player = {
	moving = false,
	prev_moving = false,
	animation1 = "LEGS_RUN",
	animation2 = "TORSO_STAND",
	gesture = false,
	prev_gesture = false,
	shot = false,
	prev_shot = false,
	shots = {}
}

function player:moveForward()
	player.moving = true
	player.animation1 = "LEGS_RUN"
	qE.moveForward()
end
function player:moveBackward()
	player.moving = true
	player.animation1 = "LEGS_BACK"
	qE.moveBackward()
end
function player:strafeLeft()
	player.moving = true
	player.animation1 = "LEGS_RUN"
	qE.strafeLeft()
end
function player:strafeRight()
	player.moving = true
	player.animation1 = "LEGS_RUN"
	qE.strafeRight()
end
function player:gesture()
	player.gesture = true
	player.animation2 = "TORSO_GESTURE"
end
function player:shot()
	player.shot = true
	player.animation2 = "TORSO_ATTACK"

end

function addShot()
		local _model = getModel(debugModel)
		local _shot = {
			x = _model.pos.x,
			y = _model.pos.y,
			z = _model.pos.z,
			x2 = _model.pos.x-20,
			y2 = _model.pos.y-25,
			z2 = _model.pos.z,
			rz = data.camera.angle.z,
			ry = 0,
			rx = data.camera.angle.x,
			time = 100,
			distance = 0,
		}
		--print (_shot.x,_shot.y,_shot.z)
		--table.insert(player.shots,shot)
		player.shots[tostring(data.counter)] = _shot
		
end

function processShots()

	for a,b in pairs(player.shots) do
		--b.x = b.x2 - math.cos((b.rz-90)/360*2*3.142)*math.sin(b.rx/360*2*3.142)*b.distance
		--b.y = b.y2 + math.sin((b.rz-90)/360*2*3.142)*math.sin(b.rx/360*2*3.142)*b.distance
		b.x = b.x2 + math.cos((b.rz-90)/360*2*3.142)*b.distance
		b.y = b.y2 + math.sin((b.rz-90)/360*2*3.142)*b.distance
		--b.x = b.x2 + math.cos((b.rx)/360*2*3.142)*b.distance
		--b.y = b.y2 + math.sin((b.rx)/360*2*3.142)*b.distance
		--b.z = b.z2 + math.cos(b.rx/360*2*3.142)*b.distance

		b.distance = b.distance + 5
		if (b.time>0) then
			b.time = b.time - 1
		else
			player.shots[a] = nil	
		end
	end

end