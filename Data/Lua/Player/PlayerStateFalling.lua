function FallingFirst(args)
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(7,1.0,0.1)
end

function FallingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	--// Calculate the player speed
	local l_PlayerDisplacement = Vect3f(l_Player.m_Velocity.x, l_Player.m_Velocity.y + l_Player.m_Gravity * _ElapsedTime, l_Player.m_Velocity.z)
	
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
		local l_ObjectPosition = l_Owner:get_left_object_position()
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
		local l_ObjectRotation = l_Owner:get_left_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function FallingEnd(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function ANYToFallingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Velocity.y < -0.25
end

function FallingToIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Velocity.y == 0
end