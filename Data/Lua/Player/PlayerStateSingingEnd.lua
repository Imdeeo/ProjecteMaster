function SingingEndFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:execute_action(30, 0.1, 0.1, 1.0, true)
	l_Player.m_Timer = 0.0
	--Launch sound

	l_Player.m_SoundManager:broadcast_rtpc_value(g_IsolationRTPC, 0)
	l_Player.m_SoundManager:broadcast_state(g_ExplorationActionState)
end

function SingingEndUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
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

function SingingEndEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Player.m_IsSinging = false
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function SingingEndToFallingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= 3.0
end
