function IdleFirst(args)
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	
	--// Calculate the player speed
	local l_PlayerDisplacement = Vect3f(0, g_Player.m_Velocity.y + g_Player.m_Gravity * _ElapsedTime, 0)
	
	--// Move the character controller
	local l_PreviousControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - 0.9
	g_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	--local l_NewControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
	--l_NewControllerPosition.y = l_NewControllerPosition.y - 0.9
	--l_Owner:set_position(l_NewControllerPosition)
	
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
	g_Player.m_Velocity = l_Displacement/_ElapsedTime
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = g_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function IdleEnd(args)
end

function IdleToMovingCondition()
	return (g_Player.m_InputManager:is_action_active("MOVE_FWD") or g_Player.m_InputManager:is_action_active("MOVE_BACK") or g_Player.m_InputManager:is_action_active("STRAFE_LEFT") or g_Player.m_InputManager:is_action_active("STRAFE_RIGHT"))
end

function IdleToCrouchingCondition()
	return g_Player.m_InputManager:is_action_active("CROUCH")
end

function IdleToJumpingCondition()
	return g_Player.m_InputManager:is_action_active("JUMP")
end