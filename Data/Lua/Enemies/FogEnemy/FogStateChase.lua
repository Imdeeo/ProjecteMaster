function FogChaseFirst(args)
	utils_log("start chase")
	--l_Owner:blend_cycle(0,1.0,0.1)	
end

function FogChaseUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Owner:get_position()
	local l_Direction = (l_PlayerPos - l_EnemyPos):get_normalized(1)


	 
	local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
 
	local l_Angle = l_EnemyForward * l_Direction
 
	local angle_to_turn = math.acos(l_Angle)
	local cross = l_Direction ^ l_EnemyForward
	if cross.y < 0.0 then
	  angle_to_turn = -angle_to_turn
	end
	local quat_to_turn = Quatf()
	quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)
	utils_log("angle: "..angle_to_turn)
	local target_quat = l_Owner:get_rotation() * quat_to_turn
	l_Owner:set_rotation(target_quat)
	l_Owner:set_poisiton(l_Owner:get_position()+target_quat:get_forward_vector()*_ElapsedTime)
end

function FogChaseEnd(args)
	utils_log("end chase")
end

function FogChaseToTeleportCondition()
	return false
end

function FogChaseToAttackCondition()
--	local l_Enemy = m_CharacterManager.m_Enemics[1]
--	local l_Direction = (l_PlayerPos - l_EnemyPos)
--	l_Direction.y = 0
--	return l_Direction:length() < 1
return false
end

function FogChaseToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end

