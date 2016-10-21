function CorrectingFirst(args)
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	--l_Player:CalculateCameraPositionRotation(l_Player.m_CameraAnimation, l_Player.m_TargetPosOffset)
	l_Player.m_InitialCameraRotation = l_Player.m_CameraController:get_rotation()
	l_Player.m_TimerRotation = 0.0

	local l_CameraDirection = l_Player.m_TargetPosOffset*-1.0
	l_CameraDirection.y = l_Player.m_Target.y - l_Player.m_CameraController:get_position().y
	l_CameraDirection = l_CameraDirection:get_normalized(1)
						
	local quat_to_turn = Quatf()
	quat_to_turn:set_from_fwd_up(l_Player.m_ForwardCamera, l_Player.m_UpCamera)
	l_Player.m_FinalCameraRotation = quat_to_turn
end

function CorrectingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	--[[local l_AngleOK = false
	local l_PosOK = false
	
	l_Player.m_TimerRotation = l_Player.m_TimerRotation + _ElapsedTime
	local l_PercentRotation = l_Player.m_TimerRotation 
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
	end
	
	utils_log_v3(l_Player.m_Target)
	local l_FaceTargetDisplacement =  l_Player.m_Target - l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
		
	if l_FaceTargetDisplacement:length() <= 0.01 then
		l_PosOK = true
	else
		l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
	end		
	
	if l_Player.m_TimerRotation <= 1.0 then
		local target_quat = l_Player.m_InitialCameraRotation:slerpJU(l_Player.m_FinalCameraRotation, l_PercentRotation)
		l_Player.m_CameraController:set_rotation(target_quat)
	else
		l_AngleOK = true
	end		

	if l_PosOK and l_AngleOK then
		l_Player.m_IsCorrecting = false
		l_Player.m_IsInteracting = true
	end]]
	
	
	local l_AngleOK = false
	local l_PosOK = false
	
	--// Force the player face the target
		--// Movement
	local l_FaceTargetDisplacement =  l_Player.m_Target + l_Player.m_TargetPosOffset - l_Player.m_PhysXManager:get_character_controler_pos("player")
	local l_Pos = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
	if l_FaceTargetDisplacement:length() <= 0.01 then
		l_PosOK = true
	else
		l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
	end
		--// Rotation
	local l_AngleOK = l_Player:XZRotate(_ElapsedTime)
	
	if l_PosOK and l_AngleOK then
		l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement, _ElapsedTime)
		l_Player.m_CameraController:add_yaw(l_Difference*l_Dir)
		l_Player.m_IsCorrecting = false
	end
	
		--// Rotation
	--[[local l_CameraDirection = l_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0.0
	l_CameraDirection:normalize(1)
	local l_Off = l_Player.m_TargetLookOffset
	l_Off = l_Off * (-1.0)
	l_Off.y = 0.0
	l_Off:normalize(1)
	local l_Yaw = l_CameraDirection:get_angle_with(l_Off)
	local l_Dir = 1
	local l_OriginYaw
	utils_log_v3(l_Off)
	utils_log("atan2: "..math.atan2(l_CameraDirection.z, l_CameraDirection.x))
	if math.abs(l_Off.x) > math.abs(l_Off.z) then
		l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
		if l_OriginYaw > 0 then
			l_Dir = -l_Dir
		end
		if (l_Off.x < 0) then
			l_Dir = -l_Dir
		end
	else
		l_OriginYaw = math.atan2(l_CameraDirection.x, l_CameraDirection.z)
		if l_OriginYaw < 0 then
			l_Dir = -l_Dir
		end
		if (l_Off.x > 0) then
			l_Dir = -l_Dir
		end
	end
	
	if l_Yaw <= 0.01 then
		l_AngleOK = true
	else
		l_Player.m_CameraController:add_yaw(l_Yaw * _ElapsedTime)
	end]]
		

	l_Player.m_TimerRotation = l_Player.m_TimerRotation + _ElapsedTime
	local l_PercentRotation = l_Player.m_TimerRotation 
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
	end
	
	if l_Player.m_TimerRotation <= 1.0 then
		local target_quat = l_Player.m_InitialCameraRotation:slerpJU(l_Player.m_FinalCameraRotation, l_PercentRotation)
		l_Player.m_CameraController:set_rotation(target_quat)
	else
		local target_quat = l_Player.m_InitialCameraRotation:slerpJU(l_Player.m_FinalCameraRotation, 1)
		l_Player.m_CameraController:set_rotation(target_quat)
		l_AngleOK = true
	end	
		
		
	if l_PosOK and l_AngleOK then
		--l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement, _ElapsedTime)
		--l_Player.m_CameraController:add_yaw(l_Yaw)
		l_Player.m_IsCorrecting = false
	end]]
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)

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

function CorrectingEnd(args)
	
end

function ANYToCorrectingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsCorrecting
end

function CorrectingToPuzzleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsPuzzle and not l_Player.m_IsCorrecting
end

function CorrectingToInteractingCondition(args)
	local l_Player = args["self"]
	--return false
	return l_Player.m_IsInteracting and not l_Player.m_IsCorrecting
end
