dofile("Data\\Lua\\Player\\Helpers.lua")

function ClimbingStartFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	l_Owner:blend_cycle(1,1.0,0.1)
	l_Player.m_Timer = 0.0
	l_Player.m_AnimationTime = 0.6666667
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
	local l_CameraDirection = l_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0.0
	l_CameraDirection:normalize(1)
	local l_Off = l_Player.m_TargetLookOffset
	local l_Yaw = l_CameraDirection:get_angle_with(l_Off)
	
	local l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
	if l_OriginYaw > -2.36 and l_OriginYaw < 0.0 then
		l_Yaw = l_Yaw * (-1.0)
	end
	
	if l_Yaw > 0.01 or l_Yaw < -0.01 then
		l_Player.m_CameraController:add_yaw(l_Yaw * _ElapsedTime)
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
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.3)
end

function ANYToClimbingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsClimbing
end

function ClimbingStartToClimbingIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime
end
