function SingingStartFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsSinging = true
	l_Owner:execute_action(28, 0.1, 0.1, 1.0, true)
	l_Player.m_Timer = 0.0
	l_Player.m_CameraController:lock()
	l_Player.m_InitialCameraRotation = l_Player.m_CameraController:get_rotation()
	l_Player.m_TimerRotation = 0.0
	
	local l_Up = Vect3f(0.0, 1.0, 0.0)
	local l_Fwd = Vect3f(0.0, 0.0, 0.0)
	l_Fwd = l_Player.m_CameraController:get_forward()
	l_Fwd.y = 0.0
	l_Fwd:normalize(1)
	l_Fwd.y = -1.1
	l_Fwd:normalize(1)
	local quat_to_turn = Quatf()
	quat_to_turn:set_from_fwd_up(l_Fwd, l_Up)
	l_Player.m_FinalCameraRotation = quat_to_turn
	l_Player.m_SoundManager:broadcast_rtpc_value(g_IsolationRTPC, 45)
	l_Player.m_SoundManager:broadcast_state(g_SingingActionState)
end

function SingingStartUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	--utils_log("pene")
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	if not l_Player.m_InputManager:is_action_active("Sing") then
		l_Player.m_IsSinging = false
	end
	
		--// Rotation
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

function SingingStartEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function SingingStartToSingingLoopCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= 1.0
end

function SingingStartToSingingEndCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_InputManager:is_action_active("Sing")
end

function ANYToSingingStartCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("Sing")
end
