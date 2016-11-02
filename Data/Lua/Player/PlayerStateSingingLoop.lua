function SingingLoopFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(29,1.0,0.1)
	--Launch sound
	l_Player.m_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0.5, l_Owner)
	l_Player.m_SoundManager:play_event(g_SingSoundEvent, l_Owner)

	l_Player.m_Timer = l_Player.m_SanityGainTick --always ticks on start
end

function SingingLoopUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	if l_Player.m_Timer >= l_Player.m_SanityGainTick then
		l_Player:ModifySanity(l_Player.m_SanityGain * l_Player.m_SanityGainTick)
		l_Player.m_Timer = l_Player.m_Timer - l_Player.m_SanityGainTick
	end
	
	if not l_Player.m_InputManager:is_action_active("Sing") then
		l_Player.m_IsSinging = false
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

function SingingLoopEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	l_Player.m_SoundManager:play_event(g_StopSingingSoundEvent, l_Owner)
end

function SingingLoopToSingingEndCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_InputManager:is_action_active("Sing")
end
