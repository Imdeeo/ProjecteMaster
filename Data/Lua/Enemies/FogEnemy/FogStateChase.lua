function FogChaseFirst(args)
	utils_log("Chase First")
end

function FogChaseUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	--l_Owner:set_position(l_Owner:get_position() + l_Owner:get_rotation():get_forward_vector() * _ElapsedTime * l_Enemy.m_Speed
	--local l_NewControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
end

function FogChaseEnd(args)
	utils_log("Chase end")
end

function FogChaseToTeleportCondition()
	return false
end

function FogChaseToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end

function FogChaseToAttackCondition()
	return false
end
