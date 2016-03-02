-- EnemyUtils

function go2point(args,_ElapsedTime)

	
	local _owner = args["owner"]
	local _point2go = args["point2go"]
	local _enemy_name = args["enemy_name"]
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	
	local enemyPos = l_physXManager:get_character_controler_pos(_enemy_name);

	local move = _point2go - enemyPos;
	l_physXManager:character_controller_move(_enemy_name, move*0.5, _ElapsedTime);
	local l_PosCharacterController = l_physXManager:get_character_controler_pos(_enemy_name)
	_owner:set_position(l_PosCharacterController)	

	local dir = Vect3f(math.cos(_owner:get_yaw()),0,-math.sin(_owner:get_yaw()));
	--utils_log(type(enemyPos))
	--utils_log(type(move))
	local yaw = _owner:get_yaw()
	local dot_result = dir.x*move.x + dir.y*move.y + dir.z*move.z
	--local cross_result = Vect3f(dir.y*move.z - dir.z* move.y, dir.z*move.x-dir.x*move.z, dir.x*move.y-dir.y*move.x)
	local y_cross = dir.z*move.x-dir.x*move.z
	--if y_cross >= 0 then
		_owner:set_yaw(yaw -dot_result*_ElapsedTime)
	--else
		--_owner:set_yaw(yaw + dot_result*_ElapsedTime)
	--end

--	_owner:set_yaw(
end

function modulus(vect)
	local x = vect.x*vect.x
	local y = vect.y*vect.y
	local z = vect.z*vect.z
	local result = math.sqrt(x+y+z)
	return result
end

waypoints = {}
waypoints [0] = Vect3f(18,0,18)
waypoints [1] = Vect3f(18,0,-18)
waypoints [2] = Vect3f(-18,0,-18)
waypoints [3] = Vect3f(-18,0,18)

nwaypoints = 4

actual_waypoint = 0

mode = "circular"

step = 1

arrive_value = 0.5

function get_next_waypoint (_actual_waypoint)
	local next_waypoint = _actual_waypoint + step
	if(step>0)then
		if(next_waypoint>=nwaypoints)then
			if(mode=="circular")then
				next_waypoint = 0
			end
			if(mode=="pingpong")then
				step = -step
				next_waypoint = _actual_waypoint + step
			end
		end
	else
		if(next_waypoint<=nwaypoints)then
			if(mode=="circular")then
				next_waypoint = nwaypoints - 1
			end
			if(mode=="pingpong")then
				step = -step
				next_waypoint = _actual_waypoint + step
			end
		end
	end
	return next_waypoint
end