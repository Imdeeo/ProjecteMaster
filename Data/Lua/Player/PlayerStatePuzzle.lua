function PuzzleFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]

	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	l_Player.m_Timer = -22.5
	l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, true)
end

function PuzzleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	if l_Player.m_IsPuzzle then
		local l_Camera = CUABEngine.get_instance():get_camera_controller_manager():get_resource(l_Player.m_CameraAnimation)
		l_Camera:force_update_yaw(_ElapsedTime)
		--// Raycast
		local l_PlayerPos = l_Player.m_PhysXManager:get_character_controler_pos("player")
		CheckRaycast(l_Player, l_PlayerPos, l_Camera, l_Owner)
	else
		if l_Player.m_Timer == 0.0 then
			l_Owner:remove_action(l_Owner:get_actual_action_animation())
			l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
			l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, true)
		end
		l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
		if l_Player.m_Timer >= -20.0 and l_Player.m_Timer < 0.0 then
			l_Player.m_Timer = 0.0
		end
	end
end

function PuzzleEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	--// Reset puzzle
	if l_Player.m_OrganKeyCount <= table_length(l_Player.m_OrganKeyOrder) then
		l_Player.m_PhysXManager:enable_trigger("TriggerPipeOrgan")
		l_Player.m_OrganKeyCount = 1
	end
	
	l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraControllerManager:get_resource(l_Player.m_CameraControllerName):copy_from_key_camera(l_CameraControllerManager:get_main_camera():get_camera_as_info())
	l_CameraControllerManager:choose_main_camera(l_Player.m_CameraControllerName)
		
	l_Player.m_CurrentAend = nil
	l_Player.m_Target = nil
	l_Player.m_TargetOffset = Vect3f(0.0, 0.0, 0.0)
	l_Player.m_InteractingAnimation = 0
	l_Player.m_InteractingCinematic = nil
	l_Player.m_CameraAnimation = nil
	l_Player.m_IsInteracting = false
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
	l_Player.m_IsPuzzle = false
	l_Player.m_AnimationTime = 0
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
	
	utils_log("PuzzleEnd")
end

function PuzzleToFallingCondition(args)
	local l_Player = args["self"]
	return ((l_Player.m_Timer > l_Player.m_AnimationTime) and not l_Player.m_IsPuzzle) or l_Player.m_InputManager:is_action_active("MoveBackward")
end