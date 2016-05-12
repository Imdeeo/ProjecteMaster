function JumpingFirst(args)
	g_Player.m_Velocity.y = 4.9
	g_Player.m_IsJumping = true
	g_Player.m_IsAscending = true
	utils_log("JumpingFirst")
end

function JumpingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	
	--// Manage player jump	
	if(g_Player.m_IsAscending and g_Player.m_Velocity.y < 0.01) then
		g_Player.m_IsAscending = false
	end
	
	if((not g_Player.m_IsAscending) and g_Player.m_Velocity.y == 0)then
		g_Player.m_IsJumping = false
	end
	
	--// Calculate the player speed
	local l_PlayerDisplacement = Vect3f(g_Player.m_Velocity.x * 0.985, g_Player.m_Velocity.y + g_Player.m_Gravity * _ElapsedTime, g_Player.m_Velocity.z * 0.985)
	
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
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1);
	if l_Displacement.y == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
end

function JumpingEnd(args)
	utils_log("JumpingEnd")
end

function JumpingToIdleCondition()
	return not g_Player.m_IsJumping
end