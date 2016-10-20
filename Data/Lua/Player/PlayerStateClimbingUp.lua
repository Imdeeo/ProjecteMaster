dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingUpFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(19,1.0,0.3)
	l_Player.m_Timer = 0.0
	l_Player.m_AnimationTime = 1.3333333
	l_Player.m_ClimbingUp = true
end

function ClimbingUpUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_PlayerDisplacement = Vect3f(0.0, 0.0, 0.0)
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	--// Move player vertically
	l_PlayerDisplacement.y = l_PlayerDisplacement.y + (144.0 * _ElapsedTime)
	
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

function ClimbingUpEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_ClimbingUp = false
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.3)
end

function ClimbingUpToClimbingIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime
end
