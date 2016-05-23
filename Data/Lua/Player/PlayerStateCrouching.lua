function CrouchingFirst(args)
	g_Player.m_PhysXManager:set_character_controller_height("player", 1)
	utils_log("CrouchingFirst")
	local l_switch = SoundSwitch()
	l_switch.switch_name = "music_mode"
	local l_switchvalue = SoundSwitchValue()
	l_switchvalue.sound_switch = l_switch
	l_switchvalue.value_name = "action"
	g_Player.m_SoundManager:set_switch(l_switchvalue)
end

function CrouchingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_ForwardMovement = g_Player.m_InputManager:get_axis("MOVE_FWD")
	local l_StrafeMovement = g_Player.m_InputManager:get_axis("STRAFE")
	local l_Speed = g_Player.m_Speed
	--// Detect if player is crouching backwards
	if g_Player.m_InputManager:is_action_active("MOVE_BACK") then
		l_Speed = l_Speed * 0.15
	else
		l_Speed = l_Speed * 0.3
	end
	
	--// Move player forward and laterally
	local l_CameraDirection = g_Player.m_CameraController:get_forward():get_normalized(1)
	local l_CameraXZDirection = Vect2f(l_ForwardMovement * l_CameraDirection.x, l_ForwardMovement * l_CameraDirection.z)
	local l_CameraXZDirectionOrt = Vect2f(-l_CameraDirection.z * l_StrafeMovement, l_StrafeMovement * l_CameraDirection.x)
	local l_NewPlayerDirection = Vect2f(l_CameraXZDirection.x + l_CameraXZDirectionOrt.x, l_CameraXZDirection.y + l_CameraXZDirectionOrt.y)
	local l_PlayerDisplacement = Vect3f(l_NewPlayerDirection.x * l_Speed, g_Player.m_Velocity.y + g_Player.m_Gravity * _ElapsedTime, l_NewPlayerDirection.y * l_Speed)
	
	--// Move the character controller
	local l_PreviousControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - 0.9
	g_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - 0.9
	l_Owner:set_position(l_NewControllerPosition)
	
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
	g_Player.m_Velocity = l_Displacement/_ElapsedTime
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = g_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
	
	--// Check if player had displacement, to animate it or not
	local l_X = l_Displacement.x*l_Displacement.x
	--local l_Y = l_Displacement.y*l_Displacement.y
	local l_Y = 0
	local l_Z = l_Displacement.z*l_Displacement.z
	local l_DisplacementModule = math.sqrt(l_X + l_Y + l_Z)
	
	--// Animate player
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	if l_DisplacementModule == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
end

function CrouchingEnd(args)
	g_Player.m_PhysXManager:set_character_controller_height("player", 2)
	utils_log("CrouchingEnd")
	local l_switch = SoundSwitch()
	l_switch.switch_name = "music_mode"
	local l_switchvalue = SoundSwitchValue()
	l_switchvalue.sound_switch = l_switch
	l_switchvalue.value_name = "exploration"
	g_Player.m_SoundManager:set_switch(l_switchvalue)
end

function CrouchingToIdleCondition()
	return not (g_Player.m_InputManager:is_action_active("CROUCH"))
end
