function MovingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	if l_Player.m_CurrentAnimation == "move" then
		l_Owner:blend_cycle(0,1.0,0.1)
	elseif l_Player.m_CurrentAnimation == "run" then
		l_Owner:blend_cycle(2,1.0,0.1)
	else
		l_Owner:blend_cycle(0,1.0,0.1)
		l_Player.m_CurrentAnimation = "move"
	end
end

function MovingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_ForwardMovement = l_Player.m_InputManager:get_axis("MOVE_FWD")
	local l_StrafeMovement = l_Player.m_InputManager:get_axis("STRAFE")
	local l_Speed = l_Player.m_Speed
	
	--// Detect if player is moving backwards, walking, or running, then assign animation accordingly.
	if l_Player.m_InputManager:is_action_active("MOVE_BACK") and not l_Player.m_InputManager:is_action_active("RUN") then
		l_Speed = l_Speed * 0.5
	end
	
	l_Player.m_LastAnimation = l_Player.m_CurrentAnimation
	if l_Player.m_InputManager:is_action_active("RUN") then
		l_Speed = l_Speed * 2
		l_Player.m_CurrentAnimation = "run"
	else
		l_Player.m_CurrentAnimation = "move"
	end
	
	--// Move player forward and laterally
	local l_CameraDirection = l_Player.m_CameraController:get_forward():get_normalized(1)
	local l_CameraXZDirection = Vect2f(l_ForwardMovement * l_CameraDirection.x, l_ForwardMovement * l_CameraDirection.z)
	local l_CameraXZDirectionOrt = Vect2f(-l_CameraDirection.z * l_StrafeMovement, l_StrafeMovement * l_CameraDirection.x)
	local l_NewPlayerDirection = Vect2f(l_CameraXZDirection.x + l_CameraXZDirectionOrt.x, l_CameraXZDirection.y + l_CameraXZDirectionOrt.y)
	local l_PlayerDisplacement = Vect3f(l_NewPlayerDirection.x * l_Speed, l_Player.m_Velocity.y + l_Player.m_Gravity * _ElapsedTime, l_NewPlayerDirection.y * l_Speed)
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - 0.9
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - 0.9
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
end

function MovingEnd(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function MovingToIdleCondition(args)
	local l_Player = args["self"]
	return not (l_Player.m_InputManager:is_action_active("MOVE_FWD") or l_Player.m_InputManager:is_action_active("MOVE_BACK") or l_Player.m_InputManager:is_action_active("STRAFE_LEFT") or l_Player.m_InputManager:is_action_active("STRAFE_RIGHT"))
end

function MovingToCrouchingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("CROUCH")
end

function MovingToJumpingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("JUMP")
end
