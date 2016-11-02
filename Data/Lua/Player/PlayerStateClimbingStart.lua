dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingStartFirst(args)
	utils_log("ClimbingStartFirst")
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.0)
	l_Owner:execute_action(18,0.1, 0.1, 1.0, true)
	l_Player.m_Timer = 0.0
	l_Player.m_AnimationTime = 0.6666667
	l_Player.m_InitialCameraRotation = l_Player.m_CameraController:get_rotation()
	
	local l_Up = Vect3f(0.0, 1.0, 0.0)
	local quat_to_turn = Quatf()
	quat_to_turn:set_from_fwd_up(l_Player.m_ForwardCamera, l_Up)
	l_Player.m_FinalCameraRotation = quat_to_turn
end

function ClimbingStartUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	local l_PlayerDisplacement = Vect3f(0.0, 0.0, 0.0)
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	--// Force the player face the target
		--// Movement
	local l_FaceTargetDisplacement =  l_Player.m_Target + l_Player.m_TargetPosOffset - l_Player.m_PhysXManager:get_character_controler_pos("player")
	local l_Pos = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
	if l_FaceTargetDisplacement:length() > 0.03 then
		l_PlayerDisplacement = l_PlayerDisplacement + l_FaceTargetDisplacement:get_normalized(1)
	end
	
		--// Rotation
	local l_PercentRotation = l_Player.m_Timer / l_Player.m_AnimationTime
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
	end
	
	if l_Player.m_TimerRotation <= 1.0 then
		local target_quat = l_Player.m_InitialCameraRotation:slerpJU(l_Player.m_FinalCameraRotation, l_PercentRotation)
		l_Player.m_CameraController:set_rotation(target_quat)
	else
		local target_quat = l_Player.m_InitialCameraRotation:slerpJU(l_Player.m_FinalCameraRotation, 1)
		l_Player.m_CameraController:set_rotation(target_quat)
	end	
	
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

function ClimbingStartEnd(args)
	utils_log("ClimbingStartEnd")
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function ANYToClimbingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsClimbing
end

function ClimbingStartToClimbingIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime
end
