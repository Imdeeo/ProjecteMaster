dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingIdleFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	--l_Player.m_IsClimbing = true
	l_Player.m_Timer = -1.0
	l_Player.m_ClimbingUp = false
	l_Player.m_ClimbingDown = false
	l_Owner:blend_cycle(21,1.0,0.1)
end

function ClimbingIdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
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

function ClimbingIdleEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_ClimbingUp = false
	l_Player.m_ClimbingDown = false
	if not l_Player.m_IsClimbing and not l_Player.m_IsInteracting then
		ClearPlayerTarget(l_Player)
		l_Player.m_CameraController:unlock()
	end
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.3)
end

function ClimbingIdleToClimbingDownCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("MoveBackward") and l_Player.m_IsClimbing
end

function ClimbingIdleToClimbingUpCondition(args)
	local l_Player = args["self"]
	return l_Player.m_InputManager:is_action_active("MoveForward") and l_Player.m_IsClimbing
end

function ClimbingIdleToInteractingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsInteracting
end

function ClimbingIdleToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsClimbing and not l_Player.m_IsInteracting
end
