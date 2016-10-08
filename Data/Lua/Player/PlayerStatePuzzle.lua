function PuzzleFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]

	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	l_Player.m_Timer = 0.0

	if l_Player.m_CameraAnimation ~= nil then
		l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, true)
		utils_log("Animated Camera: "..l_Player.m_CameraAnimation)
	else
		utils_log("Animated Camera: Null")
	end
end

function PuzzleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	if l_Player.m_IsPuzzle then
		if l_Player.m_Timer > l_Player.m_AnimationTime then
			if l_Player.m_CameraAnimation ~= nil then
				--l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
				--l_CameraControllerManager:get_resource(l_Player.m_CameraControllerName):copy_from_key_camera(l_CameraControllerManager:get_main_camera():get_camera_as_info())
				--l_CameraControllerManager:choose_main_camera(l_Player.m_CameraControllerName)
				l_Player.m_CameraAnimation = nil
				--l_Player.m_CameraController:unlock()
			end
			-- move camera horizontally
		end
	else
		if l_Player.m_Timer == 0.0 then
			l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	
			if l_Player.m_CameraAnimation ~= nil then
				l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, true)
				utils_log("Animated Camera: "..l_Player.m_CameraAnimation)
			else
				utils_log("Animated Camera: Null")
			end
		end
	end
end

function PuzzleEnd(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
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
	return ((l_Player.m_Timer > l_Player.m_AnimationTime) and not l_Player.m_IsPuzzle)
end