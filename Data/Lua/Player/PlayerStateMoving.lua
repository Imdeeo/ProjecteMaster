function MovingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	if l_Player.m_CurrentAnimation == "move" then
		l_Owner:blend_cycle(0,1.0,0.1)
	elseif l_Player.m_CurrentAnimation == "run" then
		l_Owner:blend_cycle(26,1.0,0.1)
	else
		l_Owner:blend_cycle(0,1.0,0.1)
		l_Player.m_CurrentAnimation = "move"
	end
end

function MovingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_ForwardMovement = 0
	local l_StrafeMovement = 0
	if l_Player.m_InputManager:is_action_active("MoveForward") then 
		l_ForwardMovement = l_ForwardMovement+1
	end
	if l_Player.m_InputManager:is_action_active("MoveBackward") then
		l_ForwardMovement = l_ForwardMovement-1
	end
	if l_Player.m_InputManager:is_action_active("StrafeLeft") then
		l_StrafeMovement = l_StrafeMovement-1
	end
	if l_Player.m_InputManager:is_action_active("StrafeRight") then
		l_StrafeMovement = l_StrafeMovement+1
	end
	local l_Speed = l_Player.m_Speed

	--// Detect if player is moving backwards, walking, or running.
	if l_Player.m_InputManager:is_action_active("MoveBackward") and not l_Player.m_InputManager:is_action_active("Run") then
		l_Speed = l_Speed * 0.5
	end
	
	l_Player.m_LastAnimation = l_Player.m_CurrentAnimation
	if l_Player.m_InputManager:is_action_active("Run") then
		l_Speed = l_Speed * 2
		l_Player.m_CurrentAnimation = "run"
	else
		l_Player.m_CurrentAnimation = "move"
	end
	
	--// Move player forward and laterally
	local l_CameraDirection = l_Player.m_CameraController:get_forward():get_normalized(1)
	local l_CameraXZDirection = Vect2f(l_ForwardMovement * l_CameraDirection.x, l_ForwardMovement * l_CameraDirection.z)
	local l_CameraXZDirectionOrt = Vect2f(-l_CameraDirection.z * l_StrafeMovement, l_StrafeMovement * l_CameraDirection.x)
	local l_NewPlayerDirection = Vect2f(l_CameraXZDirection.x + l_CameraXZDirectionOrt.x, l_CameraXZDirection.y + l_CameraXZDirectionOrt.y)
	local l_PlayerDisplacement = Vect3f(l_NewPlayerDirection.x * l_Speed, l_Player.m_Velocity.y + l_Player.m_Gravity * _ElapsedTime, l_NewPlayerDirection.y * l_Speed)
	
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

function MovingEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	g_Engine:get_level_manager():get_level(l_Player.m_ActualLevel):get_layer_manager():get_layer("interactuable_objects"):destroy(false)
end

function MovingToIdleCondition(args)
	local l_Player = args["self"]
	return not (l_Player.m_InputManager:is_action_active("MoveForward") or l_Player.m_InputManager:is_action_active("MoveBackward") or l_Player.m_InputManager:is_action_active("StrafeLeft") or l_Player.m_InputManager:is_action_active("StrafeRight"))
end

function MovingToCrouchingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("Crouch")
end

function MovingToJumpingCondition(args)
	local l_Player = args["self"]
	if l_Player.m_InputManager:is_action_active("Jump") then
		if l_Player.m_InputManager:is_action_active("Run") then
			l_Player.m_Velocity.y = 4.9
		else
			l_Player.m_Velocity.y = 4.0
		end
		return true
	else
		return false
	end
end
