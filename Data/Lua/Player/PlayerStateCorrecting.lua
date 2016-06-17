function CorrectingFirst(args)
utils_log("CorrectingFirst")
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
end

function CorrectingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	--// Force the player face the target
		--// Movement
	local l_FaceTargetDisplacement =  l_Player.m_Target - l_Player.m_PhysXManager:get_character_controler_pos("player") + l_Player.m_TargetOffset
	l_FaceTargetDisplacement.y = 0.0
	l_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)

		--// Rotation
	local l_CameraDirection = l_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0.0
	local l_Off = l_Player.m_TargetOffset
	l_Off = l_Off * (-1.0)
	l_Off.y = 0.0
	local l_OriginYaw = math.atan2(l_CameraDirection:get_normalized(1).z, l_CameraDirection:get_normalized(1).x)
	local l_TargetYaw = math.atan2(l_Off:get_normalized(1).z, l_Off:get_normalized(1).x)
	local l_Yaw = l_CameraDirection:get_normalized(1):get_angle_with(l_Off:get_normalized(1))
	utils_log("Origin: "..l_OriginYaw)
	utils_log("Target: "..l_TargetYaw)
	utils_log("Differ: "..l_Yaw)
	if (l_TargetYaw < l_OriginYaw) then
		l_Yaw = -l_Yaw
	end
	l_Player.m_CameraController:add_yaw(l_Yaw * _ElapsedTime)
	
	if l_FaceTargetDisplacement:length() <= 0.1 and l_Yaw <= 0.01 and l_Yaw >= -0.01 then
		CheckClimbingOrInteracting(l_Player)
	end
	--//

	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function CorrectingEnd(args)
	
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

function CorrectingToInteractingCondition(args)
	local l_Player = args["self"]
	return (l_Player.m_IsInteracting and l_Player.m_IsClimbing)
end

function CorrectingToClimbingCondition(args)
	local l_Player = args["self"]
	return (not l_Player.m_IsInteracting and not l_Player.m_IsClimbing)
end
