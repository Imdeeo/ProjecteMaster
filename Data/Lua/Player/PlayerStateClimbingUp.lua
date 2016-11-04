dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingUpFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:blend_cycle(19,0.01,0.01)
	l_Player.m_Timer = 0.0
	l_Player.m_AnimationTime = 1.333333
	l_Player.m_ClimbingUp = true
	utils_log("ClimbingUpFirst")
end

function ClimbingUpUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_PlayerDisplacement = Vect3f(0.0, 0.0, 0.0)
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	--// Move player vertically
	local l_Dist = 0.804
	local l_Speed = l_Dist/l_Player.m_AnimationTime
	l_PlayerDisplacement.y = l_Speed 
	
	--// Move the character controller
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
	--l_Owner:set_rotation(l_RotationY)
end

function ClimbingUpEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	utils_log("ClimbingUpEnd")
	l_Player.m_ClimbingUp = false
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.3)
end

function ClimbingUpToClimbingIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime
end

function ClimbingUpToFinishCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Finish
end
