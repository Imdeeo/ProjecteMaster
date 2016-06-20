function InteractingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsClimbing = false
	l_Owner:blend_cycle(l_Player.m_InteractingAnimation, 1.0, 0.1)
	if (not l_Player.m_InteractingCinematic == nil) then
		m_CinematicManager.m_CurrentAction = l_Player.m_InteractingCinematic
		m_CinematicManager.m_Play = true
	end
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function InteractingEnd(args)
	local l_Player = args["self"]
	l_Player.m_Target = nil
	l_Player.m_IsCorrecting = false
	l_Player.m_InteractingAnimation = 0
	l_Player.m_InteractingCinematic = nil
	l_Player.m_CameraController:unlock()
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(), 0.1)
end

function InteractingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting
end
