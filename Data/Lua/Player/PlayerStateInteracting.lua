function InteractingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_IsClimbing = false
	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):play() --OJO! Lanzar cinemática, y asignar m_CinematicDuration.
	end
	m_CinematicDuration = l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):get_duration()
	utils_log("Cinematic duration: "..m_CinematicDuration)
	m_Timer = 0.0
	if l_Player.m_ItemName ~= nil then
		l_Player.m_Item = CUABEngine.get_instance():get_layer_manager():get_resource("solid"):get_resource(l_Player.m_ItemName)
	end
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	--// Ends the state after the animation duration has passed
	m_Timer = m_Timer + _ElapsedTime
	if m_Timer >= m_CinematicDuration then
		l_Player.m_IsInteracting = false
	end

	--// If player has an item, move it.
	if m_Timer >= 1.5 and l_Player.m_Item ~= nil then
		local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - 0.45
		local l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_Owner:get_right_object_position())
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_ObjectPosition+l_NewControllerPosition
		l_Player.m_Item:set_position(l_ObjectPosition)
		local l_ObjectRotation = l_Owner:get_right_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
end

function InteractingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Player.m_Target = nil
	l_Player.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
	l_Player.m_InteractingAnimation = 0
	l_Player.m_InteractingCinematic = nil
	l_Player.m_IsInteracting = false
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
end

function InteractingToFallingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting
end
