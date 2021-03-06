function FogTeleportFirst(args)
	local l_Enemy = args["self"]
	l_Enemy.m_teleport_timer = 0
	l_Enemy.m_chase = false
end

function FogTeleportUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
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
end

function FogTeleportToOffCondition(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_off == true
end

function FogTeleportToChaseCondition(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_chase == true
end

