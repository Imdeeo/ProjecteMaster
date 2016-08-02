function ClimbingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsClimbing = true
	l_Owner:blend_cycle(1,1.0,0.1);
end

function ClimbingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
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
	local l_PlayerDisplacement = Vect3f(0, l_Speed * l_ForwardMovement * _ElapsedTime, 0)
	
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
		local l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_Owner:get_right_object_position())
		--l_ObjectPosition = Vect3f(-l_ObjectPosition.x, l_ObjectPosition.z, l_ObjectPosition.y)
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_ObjectPosition+l_NewControllerPosition
		l_Player.m_Item:set_position(l_ObjectPosition)
		local l_ObjectRotation = l_Owner:get_right_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function ClimbingEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_Target = nil
	l_Player.m_IsInteracting = false
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
	l_Player.m_CameraController:unlock()
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function ClimbingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsClimbing
end
