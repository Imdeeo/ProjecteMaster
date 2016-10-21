function IdleFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(0,1.0,0.1)
	l_Player.m_PhysXManager:set_character_controller_height("player", g_Height)
	--local l_Pos = l_Player.m_PhysXManager:get_character_controler_pos("player")
	--utils_log("Pos: "..l_Pos.x..", "..l_Pos.y..", "..l_Pos.z)
	l_Player.m_CameraController:unlock()
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
		
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
	CheckRaycast(l_Player, l_Player.m_CameraController, l_Owner)
	
	
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition	
	l_Player.m_Velocity = l_Displacement/_ElapsedTime

    if l_Player.m_Velocity.y > 0 then
        l_Player.m_Velocity.y = 0
    end	
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	
	l_RotationY = Quatf()
	
	l_Rotation = l_Player.m_CameraController:get_rotation()
	
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	
	l_Owner:set_rotation(l_RotationY)
	
	--// If player has an item, move it.
	if l_Player.m_Item ~= nil then
		local l_CameraDirection = l_Player.m_CameraController:get_forward()
		local l_ObjectPosition
		local l_ObjectRotation
		if l_Player.m_LeftHanded == false then 
			l_ObjectPosition = l_Owner:get_right_object_position()
			l_ObjectRotation = l_Owner:get_right_object_rotation()
		else
			l_ObjectPosition = l_Owner:get_left_object_position()
			l_ObjectRotation = l_Owner:get_left_object_rotation()
		end
		if math.abs(l_CameraDirection.x) > math.abs(l_CameraDirection.z) then
			l_ObjectRotation = rotate_quat_in_x_by_270(l_ObjectRotation)
			l_ObjectRotation = rotate_quat_in_y_by_180(l_ObjectRotation)
		else
			l_ObjectRotation = rotate_quat_in_z_by_180(l_ObjectRotation)
			l_ObjectRotation = rotate_quat_in_x_by_270(l_ObjectRotation)
		end
		l_ObjectRotation = l_ObjectRotation*l_Owner:get_rotation()
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
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
	if l_Player.m_InputManager:is_action_active("Jump") then
		l_Player.m_Velocity.y = 3
		return true
	else
		return false
	end
end
