function ChaseFirstAutomaton(args)
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(0,1.0,0.1)
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]	
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Owner:get_position()
	local l_Direction = (l_PlayerPos - l_EnemyPos):get_normalized(1)
		
	l_Owner:set_position(l_EnemyPos + (l_Direction * l_Enemy.m_Speed * _ElapsedTime))
	
	local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
	
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
    local target_quat = l_Owner:get_rotation() * quat_to_turn
    l_Owner:set_rotation(target_quat)
end

function ChaseEndAutomaton(args)
	utils_log("ChaseEnd")
end

function ChaseToAlertConditionAutomaton()
end

function ChaseToAttackConditionAutomaton()
end