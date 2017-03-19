-- EnemyUtils

function go2point(args,_ElapsedTime)

	local _velocity = args["velocity"]
	local _owner = args["owner"]
	local _point2go = args["point2go"]
	local _enemy_name = args["enemy_name"]
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	
	local enemyPos = l_physXManager:get_character_controler_pos(_enemy_name);

	local move = _point2go - enemyPos;
	move:normalize(1)
	l_physXManager:character_controller_move(_enemy_name, move*_velocity, _ElapsedTime);
	local l_PosCharacterController = l_physXManager:get_character_controler_pos(_enemy_name)
	_owner:set_position(l_PosCharacterController)	

	
	local dir = Vect3f(math.cos(_owner:get_yaw()),0,math.sin(_owner:get_yaw()));
	--utils_log(type(enemyPos))
	--utils_log(type(move))
	local yaw = _owner:get_yaw()
	local dot_result = dir.x*move.x + dir.y*move.y + dir.z*move.z
	--local cross_result = Vect3f(dir.y*move.z - dir.z* move.y, dir.z*move.x-dir.x*move.z, dir.x*move.y-dir.y*move.x)
	local y_cross = dir.z*move.x-dir.x*move.z
	--if y_cross >= 0 then
		_owner:set_yaw(yaw -dot_result*_ElapsedTime*_velocity)
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

class "CEnemyVision"
	function CEnemyVision:__init(_owner)
		self.m_MaxDistance = 25.0
		self.m_MaxAngle = 0.25 * math.pi
		self.m_HeadOffset = Vect3f(0.0, 1.7, 0.0)
		-- TODO: get group numbers somehow
		-- at the moment bit 0: plane, bit 1: objects, bit 2: triggers, bit 3: player
		self.m_PhysXGroups = 2 + 8 -- objects and player
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self.m_Owner = _owner

		self.m_BlockingObjectName = nil
		--self.m_Counter = 0
	end

	function CEnemyVision:PlayerVisible()

		local l_OwnerHeadPos = self.m_Owner:get_position() + self.m_HeadOffset
		local l_PlayerPos = self.m_PhysXManager:get_character_controler_pos("player")
--[[
		if self.m_Counter >= 20 then
			utils_log("enemy head y: " .. l_OwnerHeadPos.y .. " player y: " .. l_PlayerPos.y)
			self.m_Counter = 0
		end
		self.m_Counter = self.m_Counter + 1
]]
		-- not visible if too far
    local l_Dist = l_PlayerPos:distance(l_OwnerHeadPos)
    if l_Dist > self.m_MaxDistance then
      return false
    end

		-- not visible if out of angle
		local l_PlayerDirection = l_PlayerPos - l_OwnerHeadPos
    l_PlayerDirection:normalize(1.0)
    local l_Forward = self.m_Owner:get_rotation():get_forward_vector()
    local l_Dot = l_Forward * l_PlayerDirection
    if l_Dot < math.cos(self.m_MaxAngle) then
      return false
    end

		-- not visible if behind an obstacle
		-- TODO: some raycasts from enemy's head to different parts of player
    local l_RaycastData = RaycastData()
    local l_Hit = self.m_PhysXManager:raycast(
			l_OwnerHeadPos, l_PlayerPos,
			self.m_PhysXGroups, l_RaycastData
		)
    if l_Hit and l_RaycastData.actor_name ~= "player" then
      self.m_BlockingObjectName = l_RaycastData.actor_name
      return false
    end

		-- otherwise visible
		self.m_BlockingObjectName = nil
		return true
	end
