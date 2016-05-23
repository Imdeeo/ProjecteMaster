function ClimbingFirst(args)

end

function ClimbingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_ForwardMovement = g_Player.m_InputManager:get_axis("MOVE_FWD")
	local l_Speed = g_Player.m_Speed * 20
		
	--// Force the player face the target
	if not (g_Player.m_Target == nil) then
		ForcePlayerFaceTarget(_ElapsedTime)
	end
	
	--// Move player vertically
	local l_PlayerDisplacement = Vect3f(0, l_Speed * l_ForwardMovement * _ElapsedTime, 0)
	
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
	g_Player.m_Target = nil
end

function ANYToClimbingCondition()
	return g_Player.m_IsClimbing
end

function ClimbingToFallingCondition()
	return not g_Player.m_IsClimbing
end

function ClimbingToggleTrigger(_TriggerName, _ColliderName)
	if (_ColliderName == g_Player.m_Name and g_Player.m_InputManager:is_action_active("INTERACT")) then
		local l_UABEngine = CUABEngine.get_instance()
		g_Player.m_Target = l_UABEngine:get_layer_manager():get_resource("triggers"):get_resource(_TriggerName):get_position()
		g_Player.m_TargetOffset = Vect3f(1, 0, 0)
		g_Player.m_IsClimbing = not g_Player.m_IsClimbing
	end
end

function ClimbingEndTrigger(_TriggerName, _ColliderName)
	if _ColliderName == g_Player.m_Name then
		FinishClimbing()
	end
end

function FinishClimbing()
	utils_log("Finished climbing!")
	g_Player.m_IsClimbing = false
end

function ForcePlayerFaceTarget(_ElapsedTime)
	local l_FaceTargetDisplacement =  g_Player.m_Target - g_Player.m_PhysXManager:get_character_controler_pos("player") + g_Player.m_TargetOffset
	l_FaceTargetDisplacement.y = 0
	
	if l_FaceTargetDisplacement:length() <= 0.1 then
		g_Player.m_target = nil
	end
	
	g_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
	local l_CameraDirection = g_Player.m_CameraController:get_forward()
	l_CameraDirection.y = 0
	--local l_Yaw = arccos(dot(l_CameraDirection:get_normalized(1), (-m_TargetOffset):get_normalized))
end
