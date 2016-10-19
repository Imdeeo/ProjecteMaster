dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	l_Owner:blend_cycle(1,1.0,0.1)
	utils_log_v3(l_Player.m_CameraController:get_forward())
end

function ClimbingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_PlayerDisplacement = Vect3f(0.0, 0.0, 0.0)
	
	--// Force the player face the target
		--// Movement
	local l_FaceTargetDisplacement =  l_Player.m_Target + l_Player.m_TargetPosOffset - l_Player.m_PhysXManager:get_character_controler_pos("player")
	local l_Pos = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
	if l_FaceTargetDisplacement:length() > 0.03 then
		l_PlayerDisplacement = l_PlayerDisplacement + l_FaceTargetDisplacement:get_normalized(1)
	end
	
		--// Rotation
	local l_CameraDirection = l_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0.0
	l_CameraDirection:normalize(1)
	local l_Off = l_Player.m_TargetLookOffset
	local l_Yaw = l_CameraDirection:get_angle_with(l_Off)
	
	local l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
	if l_OriginYaw > -2.36 and l_OriginYaw < 0.0 then
		l_Yaw = l_Yaw * (-1.0)
	end
	
	if l_Yaw > 0.01 or l_Yaw < -0.01 then
		l_Player.m_CameraController:add_yaw(l_Yaw * _ElapsedTime)
	end
	
	local l_ForwardMovement = 0
	if l_Player.m_InputManager:is_action_active("MoveForward") then 
		l_ForwardMovement = l_ForwardMovement+1
	end
	if l_Player.m_InputManager:is_action_active("MoveBackward") then
		l_ForwardMovement = l_ForwardMovement-1
	end
	local l_Speed = l_Player.m_Speed * 20
	
	if l_Player.m_InputManager:is_action_active("Interact") then
		l_Player.m_IsClimbing = false
	end
	
	--// Move player vertically
	l_PlayerDisplacement.y = l_PlayerDisplacement.y + (l_Speed * l_ForwardMovement * _ElapsedTime)
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)
	
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

function ClimbingEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]

	--// Aend thing, this should be done after interacting
	ClearPlayerAend(l_Player)
	
	ClearPlayerTarget(l_Player)
	l_Player.m_IsClimbing = false
	l_Player.m_CameraController:unlock()
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function ANYToClimbingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsClimbing
end

function ClimbingToInteractingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsInteracting
end

function ClimbingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsClimbing
end
