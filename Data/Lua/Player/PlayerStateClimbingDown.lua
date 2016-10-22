dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingDownFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(20,1.0,0.3)
	l_Player.m_ClimbingDown = true
	l_Player.m_Timer = 0.0
	l_Player.m_AnimationTime = 1.3333333
end

function ClimbingDownUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_PlayerDisplacement = Vect3f(0.0, 0.0, 0.0)
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	--// Move player vertically
	l_PlayerDisplacement.y = l_PlayerDisplacement.y + (_ElapsedTime * (-120.0))
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)
	l_Player.m_Velocity = Vect3f(0.0, 0.0, 0.0)
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function ClimbingDownEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_ClimbingDown = false
	if not l_Player.m_IsClimbing and not l_Player.m_IsInteracting then
		l_Player:ClearTarget()
		l_Player:ClearStates()
		l_Player.m_CameraController:unlock()
	end
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.3)
end

function ClimbingDownToClimbingIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime and l_Player.m_IsClimbing
end

function ClimbingDownToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsClimbing and not l_Player.m_IsInteracting
end
