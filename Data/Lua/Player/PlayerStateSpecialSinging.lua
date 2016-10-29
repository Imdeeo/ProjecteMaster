function SpecialSingingStateFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_SingOnce = false
	l_Player.m_Timer = 0.0
	utils_log("first sing special")
	l_Owner:set_position(l_Owner:get_position()+Vect3f(0,0,0.1))
	--g_Engine:get_level_manager():unload_level("Recibidor")
end

function SpecialSingingStateUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	if l_Player.m_InputManager:is_action_active("Sing") and not l_Player.m_SingOnce then
		local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset 
		l_Owner:set_position(l_NewControllerPosition)
		--l_Owner:set_rotation(Quatf(0,1,0,0))
		l_Player.m_SingOnce = true
		l_Owner:remove_action(l_Owner:get_actual_action_animation())
		utils_log("he entrado")
		l_Owner:execute_action(31, 0.1, 0.1, 1.0, true)
	end
	
	if l_Player.m_SingOnce then
		l_Player:ModifySanity(90/l_Player.m_AnimationTime)
		l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	end
end

function SpecialSingingStateEnd(args)
	local l_Player = args["self"]
	utils_log("end1")
	local l_Owner = args["owner"]
	l_Player.m_SingOnce = false
	l_Player.m_AnimationTime = 0
	l_Owner:remove_action(l_Owner:get_actual_action_animation())	
	l_Player:ClearCamera()
	l_Player.m_CameraController:unlock()
end

function SpecialSingingStateToIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime 
end
