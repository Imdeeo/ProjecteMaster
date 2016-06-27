function IdleFirst(args)
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(0,1.0,0.1)
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	--// Calculate the player speed
	local l_PlayerDisplacement = Vect3f(0, l_Player.m_Velocity.y + l_Player.m_Gravity * _ElapsedTime, 0)
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - 0.45
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - 0.45
	l_Owner:set_position(l_NewControllerPosition)
	
	--// Raycast
	l_Player.m_RaycastData = RaycastData()
	l_Player.m_PhysXManager:raycast(l_NewControllerPosition+l_Player.m_CameraController:get_forward(), l_NewControllerPosition+(l_Player.m_CameraController:get_forward()*2), 4, l_Player.m_RaycastData)
	if l_Player.m_RaycastData.actor_name ~= "" and l_Player.m_InputManager:is_action_active("INTERACT") then
		TriggerRaycast(l_Player)
	end
	
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
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_ObjectPosition+l_NewControllerPosition
		l_Player.m_Item:set_position(l_ObjectPosition)
		local l_ObjectRotation = l_Player.m_Item:get_rotation()*l_Owner:get_right_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function IdleEnd(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function IdleToMovingCondition(args)
	local l_Player = args["self"]
	return (l_Player.m_InputManager:is_action_active("MOVE_FWD") or l_Player.m_InputManager:is_action_active("MOVE_BACK") or l_Player.m_InputManager:is_action_active("STRAFE_LEFT") or l_Player.m_InputManager:is_action_active("STRAFE_RIGHT"))
end

function IdleToCrouchingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("CROUCH")
end

function IdleToJumpingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("JUMP")
end

function TriggerRaycast(_Player)
	utils_log("Raycast interacted with: ".._Player.m_RaycastData.actor_name)
	if _Player.m_RaycastData.actor_name == "TriggerComoda" then
		_Player.m_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerComoda"):get_position()
		_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "OpenBureau"
		_Player.m_ItemName = "LlaveRecibidor"
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end