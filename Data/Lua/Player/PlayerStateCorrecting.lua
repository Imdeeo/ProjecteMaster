function CorrectingFirst(args)
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	utils_log("CorrectingFirst")
end

function CorrectingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	local l_AngleOK = false
	local l_PosOK = false
	
	--// Force the player face the target
		--// Movement
	local l_FaceTargetDisplacement =  l_Player.m_Target + l_Player.m_TargetPosOffset - l_Player.m_PhysXManager:get_character_controler_pos("player")
	local l_Pos = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
	utils_log("Distance: "..l_FaceTargetDisplacement:length())
	if l_FaceTargetDisplacement:length() <= 0.03 then
		l_PosOK = true
		utils_log("Pos OK")
	else
		l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
	end
		--// Rotation
	local l_CameraDirection = l_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0.0
	l_CameraDirection:normalize(1)
	--utils_log("Cam x: "..l_CameraDirection.x..", z:"..l_CameraDirection.z)
	local l_Off = l_Player.m_TargetLookOffset
	l_Off = l_Off * (-1.0)
	l_Off.y = 0.0
	l_Off:normalize(1)
	--utils_log("Off x: "..l_Off.x..", z:"..l_Off.z)
	local l_Yaw = l_CameraDirection:get_angle_with(l_Off)
	local l_OriginYaw
	if math.abs(l_Off.x) > math.abs(l_Off.z) then
		l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
		if l_OriginYaw > 0 then
			l_Yaw = -l_Yaw
		end
		if (l_Off.x < 0) then
			l_Yaw = -l_Yaw
		end
	else
		l_OriginYaw = math.atan2(l_CameraDirection.x, l_CameraDirection.z)
		if l_OriginYaw < 0 then
			l_Yaw = -l_Yaw
		end
		if (l_Off.x > 0) then
			l_Yaw = -l_Yaw
		end
	end
	
	if l_Yaw <= 0.01 and l_Yaw >= -0.01 then
		l_AngleOK = true
		utils_log("Angle OK")
	else
		l_Player.m_CameraController:add_yaw(l_Yaw * _ElapsedTime)
		--utils_log("Yaw: "..l_Yaw)
	end
		
	if l_PosOK and l_AngleOK then
		l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement, _ElapsedTime)
		l_Player.m_CameraController:add_yaw(l_Yaw)
		CheckClimbingOrInteracting(l_Player)
	end
	
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
	utils_log("CorrectingEnd")
end

function CheckClimbingOrInteracting(_Player)
	if (_Player.m_IsInteracting) then
		_Player.m_IsClimbing = true
	else
		_Player.m_IsClimbing = false
	end
end

function ANYToCorrectingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsCorrecting
end

function CorrectingToPuzzleCondition(args)
	local l_Player = args["self"]
	return (l_Player.m_IsPuzzle and ((l_Player.m_IsInteracting and l_Player.m_IsClimbing) or (not l_Player.m_IsInteracting and not l_Player.m_IsClimbing)))
end

function CorrectingToInteractingCondition(args)
	local l_Player = args["self"]
	return (not l_Player.m_IsPuzzle and l_Player.m_IsInteracting and l_Player.m_IsClimbing)
end

function CorrectingToClimbingCondition(args)
	local l_Player = args["self"]
	return (not l_Player.m_IsPuzzle and not l_Player.m_IsInteracting and not l_Player.m_IsClimbing)
end
