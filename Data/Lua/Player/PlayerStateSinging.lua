function SingingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsSinging = true
	
	if l_Player.m_IsWindedUp then
		l_Owner:blend_cycle(1,1.0,0.1)
	else
		l_Owner:execute_action(2, 0.1, 0.1, 1.0, true)
	end
	
	m_Timer = 0.0
end

function SingingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	m_Timer = m_Timer + _ElapsedTime
	
	if m_Timer >= 2.5 and not l_Player.m_IsWindedUp then
		l_Player.m_IsWindedUp = true
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
	if m_Timer >= 1.5 and l_Player.m_Item ~= nil then
		local l_ObjectPosition = l_Owner:get_left_object_position()
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
		local l_ObjectRotation = l_Owner:get_left_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function SingingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Player.m_CameraController:unlock()
	if l_Player.m_IsSinging then
		l_Owner:remove_action(l_Owner:get_actual_action_animation())
	else
		l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	end
end

function SingingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsSinging
end

function SingingToItselfCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsWindedUp
end

function ANYToSingingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("Sing")
end
