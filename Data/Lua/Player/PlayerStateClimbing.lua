function ClimbingFirst(args)
	utils_log("ClimbingFirst")
end

function ClimbingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_ForwardMovement = g_Player.m_InputManager:get_axis("MOVE_FWD")
	local l_Speed = g_Player.m_Speed
	
	--// Move player vertically
	local l_PlayerDisplacement = Vect3f(0, l_ForwardMovement * _ElapsedTime, 0)
	
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
		
	--// Animate player
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	if l_Displacement.y == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
end

function ClimbingEnd(args)
	utils_log("ClimbingEnd")
end

function ClimbingStartTrigger(args)
	l_Trigger = args["trigger"]
	l_Collider = args["collider"]
	if(l_Collider == g_Player and g_Player.m_InputManager:is_action_active("INTERACT")) then
		ToggleClimbing()
	end
end

function ToggleClimbing()
	if g_Player.m_PlayerStateMachine.actual_state == "climbing" then
		m_PlayerStateMachine.actual_state = "falling"
	else
		m_PlayerStateMachine.actual_State = "climbing"
	end
end

function ClimbingEndTrigger(args)
	l_Trigger = args["trigger"]
	l_Collider = args["collider"]
	if l_Collider == g_Player then
		EndClimbing()
	end
end

function EndClimbing()
	
end