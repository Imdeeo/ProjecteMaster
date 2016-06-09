function PatrolFirstAutomaton(args)
	utils_log("PatrolFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(1,0.5)
	l_Owner:blend_cycle(0,1.0,0.5)
	local l_Enemy = args["self"]
	l_Enemy.m_PathFindig:load_map("Data\\level_"..g_Engine:get_level_loaded().."\\pathfinding.xml")
	l_Enemy.m_TimerRotation = 0.0
end

function PatrolUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_EnemyPos = l_Owner:get_position()
	
	-- enemy always walks in forward direction
	local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
	l_Owner:set_position(l_EnemyPos + (l_EnemyForward * l_Enemy.m_WalkSpeed * _ElapsedTime))
	
	l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
	
	local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point("path1")
	local l_PointPos = l_NodePoint.node.position
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
	
	if l_Distance <= 2.0 and l_NodePoint.wait == false then
		l_Enemy.m_PathFindig:increment_actual_patrol_point("path1")
		l_Enemy.m_TimerRotation = 0.0
	end
		
	-- with the rotation, the enemy walks to the node point
	local l_Direction = (l_PointPos - l_EnemyPos):get_normalized(1)
	local l_Angle = l_EnemyForward * l_Direction

	if 1.0 - l_Angle < 0.01 then
	  return
	end
	
	local angle_to_turn = math.acos(l_Angle)
	local cross = l_Direction ^ l_EnemyForward
	if cross.y < 0.0 then
	  angle_to_turn = -angle_to_turn
	end
	
	local quat_to_turn = Quatf()
	quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)	
	
	local PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
	
	if PercentRotation > 1.0 then
		PercentRotation = 1.0
		l_Enemy.m_TimerRotation = 0.0
	end 
	
	local target_quat = l_Owner:get_rotation():slerp(l_Owner:get_rotation() * quat_to_turn, PercentRotation)
	l_Owner:set_rotation(target_quat)
end

function PatrolEndAutomaton(args)
end

function PatrolToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	return l_Distance < l_Enemy.m_distance_to_activate
end

function PatrolToAlertConditionAutomaton(args)
	local l_Enemy = args["self"]
	local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point("path1")
	local l_PointPos = l_NodePoint.node.position
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)
	if l_Distance >= 0.0 and l_Distance <= 0.5 then		
		if l_NodePoint.time_to_wait > 0.0 then
			utils_log("Estamos en ALERTA!!!")
			return true
		else
			utils_log("Pasamos al siguiente punto")
			l_Enemy.m_PathFindig:increment_actual_patrol_point("path1")
			return false
		end
	else
		return false
	end
end