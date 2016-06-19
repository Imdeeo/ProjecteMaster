function CrouchingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_PhysXManager:set_character_controller_height("player", 0.9)
	if l_Player.m_CurrentAnimation == "crouch_move" then
		l_Owner:blend_cycle(3,1.0,0.1)
	elseif l_Player.m_CurrentAnimation == "crouch_idle" then
		l_Owner:blend_cycle(4,1.0,0.1)
	else
		l_Owner:blend_cycle(4,1.0,0.1)
		l_Player.m_CurrentAnimation = "crouch_idle"
	end
end

function CrouchingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_ForwardMovement = l_Player.m_InputManager:get_axis("MOVE_FWD")
	local l_StrafeMovement = l_Player.m_InputManager:get_axis("STRAFE")
	local l_Speed = l_Player.m_Speed
	
	--// Detect if player is crouching backwards
	if l_Player.m_InputManager:is_action_active("MOVE_BACK") then
		l_Speed = l_Speed * 0.15
	else
		l_Speed = l_Speed * 0.3
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
	
	--// Check if player had displacement, detect if animation should change
	local l_DisplacementAmount = l_Displacement.x + l_Displacement.z
	
	--// Set player animation state
	l_Player.m_LastAnimation = l_Player.m_CurrentAnimation
	if l_DisplacementAmount == 0 then
		l_Player.m_CurrentAnimation = "crouch_idle"
	else
		l_Player.m_CurrentAnimation = "crouch_move"
	end	
end

function CrouchingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Player.m_PhysXManager:set_character_controller_height("player", 1.8)
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function CrouchingToIdleCondition(args)
	local l_Player = args["self"]
	return not (l_Player.m_InputManager:is_action_active("CROUCH"))
end
