function FogTeleportFirst(args)
	utils_log("FogTeleportFirst")
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	l_Enemy.m_teleport_timer = 0
	l_Enemy.m_chase = false
end

function FogTeleportUpdate(args, _ElapsedTime)
	--utils_log("FogTeleportUpdate")
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	l_Enemy.m_teleport_timer = l_Enemy.m_teleport_timer+_ElapsedTime
	if l_Enemy.m_teleport_timer > l_Enemy.m_time_to_teleport then
		--// Assign to the character the controller's position
		local l_NewControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition = l_NewControllerPosition + g_Player.m_RenderableObject:get_rotation():get_forward_vector() * l_Enemy.m_teleport_distance
		l_NewControllerPosition.y = l_NewControllerPosition.y - 0.9
		l_Owner:set_position(l_NewControllerPosition)
		l_Enemy.m_chase = true
	end
end

function FogTeleportEnd(args)
	utils_log("FogTeleportEnd")
end

function FogTeleportToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end

function FogTeleportToChaseCondition()
	return m_CharacterManager.m_Enemics[1].m_chase == true
end

