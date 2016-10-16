function InteractingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):play()
	end
	
	m_Timer = 0.0
	
	if l_Player.m_CameraAnimation ~= nil then
		l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, true)
	end
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	m_Timer = m_Timer + _ElapsedTime
	
	--// Ends the state after the animation duration has passed
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_IsInteracting = not l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):is_finished()
	else
		l_Player.m_IsInteracting = (m_Timer < l_Player.m_AnimationTime)
	end
	
	--// Change old to new item
	if m_Timer >= l_Player.m_ItemTime and l_Player.m_ItemTime >= 0.0 then
		l_Player.m_ItemName = l_Player.m_NewItemName
		l_Player.m_NewItemName = ""
		if l_Player.m_ItemName ~= "" then
			l_Player.m_Item = CUABEngine.get_instance():get_level_manager():get_level(l_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource(l_Player.m_ItemName)
		else
			l_Player.m_Item = nil
		end
		l_Player.m_ItemTime = l_Player.m_ItemDropTime
	end
	
	--// If player has an item, move it.
	if l_Player.m_Item ~= nil then
		local l_CameraDirection = l_Player.m_CameraController:get_forward()
		local l_ObjectPosition
		local l_ObjectRotation
		if l_Player.m_LeftHanded == false then 
			l_ObjectPosition = l_Owner:get_right_object_position()
			l_ObjectRotation = l_Owner:get_right_object_rotation()
		else
			l_ObjectPosition = l_Owner:get_left_object_position()
			l_ObjectRotation = l_Owner:get_left_object_rotation()
		end
		if math.abs(l_CameraDirection.x) > math.abs(l_CameraDirection.z) then
			l_ObjectRotation = rotate_quat_in_x_by_270(l_ObjectRotation)
			l_ObjectRotation = rotate_quat_in_y_by_180(l_ObjectRotation)
		else
			l_ObjectRotation = rotate_quat_in_z_by_180(l_ObjectRotation)
			l_ObjectRotation = rotate_quat_in_x_by_270(l_ObjectRotation)
		end
		l_ObjectRotation = l_ObjectRotation*l_Owner:get_rotation()
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function InteractingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	if l_Player.m_CameraAnimation ~= nil then
		l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
		l_CameraControllerManager:get_resource(l_Player.m_CameraControllerName):copy_from_key_camera(l_CameraControllerManager:get_main_camera():get_camera_as_info())
		l_CameraControllerManager:choose_main_camera(l_Player.m_CameraControllerName)
	end
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):stop()
	end
	if l_Player.m_CurrentAend ~= nil then
		l_Player.m_PhysXManager:character_controller_warp("player", l_Player.m_Aends[l_Player.m_CurrentAend])
		local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
		l_Owner:set_position(l_NewControllerPosition)
	end
	l_Player.m_CurrentAend = nil
	l_Player.m_Target = nil
	l_Player.m_TargetOffset = Vect3f(0.0, 0.0, 0.0)
	l_Player.m_InteractingAnimation = 0
	l_Player.m_InteractingCinematic = nil
	l_Player.m_CameraAnimation = nil
	l_Player.m_IsInteracting = false
	l_Player.m_IsPuzzle = false
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
	l_Player.m_AnimationTime = 0
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function InteractingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting
end
