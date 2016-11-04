function FocusingFirst(args)
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
	local l_Manager = CUABEngine.get_instance():get_camera_controller_manager()
	local l_Camera = l_Manager:get_resource(l_Player.m_CameraAnimation)
	l_Camera:init(l_Player.m_CameraController:get_forward(), l_Player.m_CameraController:get_up(), l_Player.m_TargetOffset, l_Player.m_CameraController:get_fov())
	l_Manager:choose_main_camera(l_Player.m_CameraAnimation)
	l_Player.m_Timer = 0.0
	l_Player.m_StartSanity = l_Player.m_Sanity
	g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_layer("solid"):get_resource("ElHijo"):set_visible(true)
end

function FocusingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]	

	l_Camera = CUABEngine.get_instance():get_camera_controller_manager():get_resource(l_Player.m_CameraAnimation)

	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	local l_Percentage = l_Player.m_Timer / l_Player.m_AnimationTime
	local l_Sanity = l_Player.m_MaxSanity - (l_Percentage * (l_Player.m_MaxSanity - l_Player.m_TargetSanity))
	if l_Sanity < l_Player.m_Sanity then
		l_Player:ModifySanity(l_Sanity-l_Player.m_Sanity)
	end
	
	if l_Player.m_Timer > l_Player.m_AnimationTime then
		l_Player.m_IsFocusing = false
	end
	
	local l_Speed = 0.04
	if l_Player.m_InputManager:is_action_active("MoveForward") then 
		l_Speed = l_Speed * 2.0
	end

	--// Move player forward
	local l_PlayerDisplacement = l_Camera:get_lookat()
	l_PlayerDisplacement.y = 0.0
	l_PlayerDisplacement:get_normalized(1)

	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement * l_Speed, _ElapsedTime)

	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)

	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Camera:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)

	--// If player has an item, move it.
	if l_Player.m_Item ~= nil then
		local l_CameraDirection = l_Camera:get_lookat()
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

function FocusingEnd(args)
	local l_Player = args["self"]
	local l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraControllerManager:get_resource(l_Player.m_CameraControllerName):copy_from_camera(l_CameraControllerManager:get_main_camera())
	l_CameraControllerManager:choose_main_camera(l_Player.m_CameraControllerName)
	l_Player.m_TargetOffset = Vect3f(0.0, 0.0, 0.0)
	l_Player.m_IsFocusing = false
	g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_layer("solid"):get_resource("ElHijo"):set_visible(false)
end

function ANYToFocusingCondition(args)
	local l_Player = args["self"]
	return l_Player.m_IsFocusing
end

function FocusingToIdleCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsFocusing
end
