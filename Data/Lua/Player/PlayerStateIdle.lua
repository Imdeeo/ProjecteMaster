function IdleFirst(args)
	utils_log("Player Idle First")
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(0,1.0,0.1)
	l_Player.m_PhysXManager:set_character_controller_height("player", g_Height)
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	utils_log("Player Idle Update")
	
	--// Calculate the player speed
	local l_PlayerDisplacement = Vect3f(0, l_Player.m_Velocity.y + l_Player.m_Gravity * _ElapsedTime, 0)
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
	
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	
	l_Owner:set_position(l_NewControllerPosition)
	
	--// Raycast
	CheckRaycast(l_Player, l_NewControllerPosition)
	
	
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
	
	l_Player.m_Velocity = l_Displacement/_ElapsedTime
	
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	
	l_RotationY = Quatf()
	
	l_Rotation = l_Player.m_CameraController:get_rotation()
	
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	
	l_Owner:set_rotation(l_RotationY)
	
	--// If player has an item, move it.
	if l_Player.m_Item ~= nil then
		local l_ObjectPosition
		if l_Player.m_LeftHanded == false then 
			l_ObjectPosition = l_Owner:get_right_object_position()
		else
			l_ObjectPosition = l_Owner:get_left_object_position()
		end
		l_ObjectPosition.x = l_ObjectPosition.x * (-1.0)
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
		local l_ObjectRotation = l_Owner:get_left_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function IdleEnd(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function IdleToMovingCondition(args)
	local l_Player = args["self"]
	return (l_Player.m_InputManager:is_action_active("MoveForward") or l_Player.m_InputManager:is_action_active("MoveBackward") or l_Player.m_InputManager:is_action_active("StrafeLeft") or l_Player.m_InputManager:is_action_active("StrafeRight"))
end

function IdleToCrouchingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("Crouch")
end

function IdleToJumpingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("Jump")
end
