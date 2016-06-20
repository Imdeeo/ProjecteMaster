function InteractingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsClimbing = false
	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	if l_Player.m_InteractingCinematic ~= nil then
		CUABEngine.get_instance():get_cinematic():play()
	end
	m_Timer = 0.0
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	m_Timer = m_Timer + _ElapsedTime
	if m_Timer >= 2.5 then
		l_Player.m_IsInteracting = false
	end
	
	--l_Player.m_IsInteracting = l_Owner:is_action_animation_active(l_Player.m_InteractingAnimation)

	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function InteractingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Player.m_Target = nil
	l_Player.m_IsCorrecting = false
	l_Player.m_InteractingAnimation = 0
	l_Player.m_InteractingCinematic = nil
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function InteractingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting
end
