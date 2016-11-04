dofile("Data\\Lua\\Sound\\Interaction.lua")

function InteractingFirst(args)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	l_Owner:execute_action(l_Player.m_InteractingAnimation, 0.1, 0.1, 1.0, true)
	
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):play()
	end

	local InteractionSoundsFunction = g_InteractionSounds[l_Player.m_InteractingAnimation]
	if InteractionSoundsFunction ~= nil then
		InteractionSoundsFunction(l_Player.m_InteractionSoundSpeaker)
	end
	
	l_Player.m_Timer = 0.0
	
	if l_Player.m_CameraAnimation ~= nil then
		l_Player:SetAnimationCamera(l_Player.m_CameraAnimation, false)
	end
	
	local l_LevelManager = CUABEngine.get_instance():get_level_manager()
	if l_Player.m_Teleport then
		if l_Player.m_ActualLevel == "Recibidor" then
			l_LevelManager:get_level("Biblioteca"):set_visible(true)
		elseif l_Player.m_ActualLevel == "Biblioteca" then
			l_LevelManager:get_level("Maquinas"):set_visible(true)
		elseif l_Player.m_ActualLevel == "Maquinas" then
			l_LevelManager:get_level("Pasillo"):set_visible(true)
			l_LevelManager:get_level("Pasillo"):set_has_to_update(true)
		end
	end
	l_Owner:set_visible(true)
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	
	-- Quitando Fog
	if l_Player.m_FogDown then
		local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
		local l_Material = l_Level:get_material_manager():get_resource("FogMaterial")

		local l_Value = 1.0-math.max(0,math.min(1,(l_Player.m_Timer-2)/5.0))

		l_Material:set_value(4, l_Value)

	end
	
	--// Ends the state after the animation duration has passed
	if l_Player.m_InteractingCinematic ~= nil then
		l_Player.m_IsInteracting = l_Player.m_Timer <= l_Player.m_CinematicManager:get_resource(l_Player.m_InteractingCinematic):get_duration()
	else
		l_Player.m_IsInteracting = (l_Player.m_Timer < l_Player.m_AnimationTime)
	end
	
	--// Change old to new item
	if l_Player.m_Timer >= l_Player.m_ItemTime and l_Player.m_ItemTime >= 0.0 then
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

function ChangeLevel(args)
	local l_Player = args["self"]
	local l_LevelManager = CUABEngine.get_instance():get_level_manager()
	if l_Player.m_ActualLevel == "Recibidor" then
		l_LevelManager:change_object_level("Recibidor","Biblioteca","solid","Puertaanimada")
		l_LevelManager:change_object_level("Recibidor","Biblioteca","solid","Pomoanimado")
		l_LevelManager:choose_scene_command_level("Biblioteca")
		g_Player:SetActualLevel("Biblioteca")			
		l_LevelManager:unload_level("Recibidor")
		l_LevelManager:get_level("Boss"):set_visible(true)
	elseif l_Player.m_ActualLevel == "Biblioteca" then
		l_LevelManager:change_object_level("Biblioteca","Maquinas","solid","PuertaSalaMaquinas")
		l_LevelManager:change_object_level("Biblioteca","Maquinas","solid","ValvulaPuertaSalaMaquinas")
		l_LevelManager:choose_scene_command_level("Maquinas")
		g_Player:SetActualLevel("Maquinas")	
		l_LevelManager:get_level("Maquinas"):set_has_to_update(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("cajacristal_SalaBoss"):set_visible(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("VidriosJaula"):set_visible(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("BajoCasaCristal"):set_visible(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("TubosGrandesSalaBoss_001"):set_visible(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("TubosGrandesSalaBoss_002"):set_visible(true)
		l_LevelManager:get_level("Boss"):get_layer_manager():get_layer("solid"):get_resource("TubosGrandesSalaBoss_003"):set_visible(true)
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("Cinta1"):play()
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("Cinta2"):play()
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("Cinta3"):play()
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("Cinta4"):play()
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("Cinta5"):play()
		l_LevelManager:unload_level("Biblioteca")
		--l_LevelManager:load_level("Pasillo",false,true,false)
	elseif l_Player.m_ActualLevel == "Maquinas" then
		l_LevelManager:change_object_level("Maquinas","Boss","solid","PuertaTaller")
		l_LevelManager:change_object_level("Maquinas","Boss","solid","PomoPuertaTaller")
		l_LevelManager:choose_scene_command_level("Boss")
		g_Player:SetActualLevel("Boss")	
		l_LevelManager:get_level("Boss"):set_has_to_update(true)
		l_LevelManager:unload_level("Maquinas")
		l_LevelManager:get_level(l_Player.m_ActualLevel):get_cinematic_manager():get_resource("engranajes_boss"):play()		
	end
end

function InteractingEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	if l_Player.m_Teleport then
		local l_AnimatedCam = g_Engine:get_camera_controller_manager():get_resource(l_Player.m_CameraAnimation)
		local l_Pos = l_AnimatedCam:get_position() - Vect3f(0,g_TotalHeight,0)
		l_Player.m_PhysXManager:character_controller_teleport("player", l_Pos)
		local l_FPS = g_Engine:get_camera_controller_manager():get_resource("MainCamera")
		l_FPS:set_position(l_AnimatedCam:get_position())
		l_FPS:set_rotation(l_AnimatedCam:get_rotation())
	end
	
	if l_Player.m_FogDown then
		local l_Level = g_Engine:get_level_manager():get_level(l_Player.m_ActualLevel)
		local l_Material = l_Level:get_material_manager():get_resource("FogMaterial")
		l_Material:set_value(0, 0.0)
		l_Player.m_FogDown = false
	end
	l_Player:ClearTarget()
	l_Player:ClearStates()
	l_Player:ClearAend(l_Owner)
	l_Player:ClearCinematic()
	
	l_Player.m_InteractingAnimation = 0
	if not l_Player.m_SingOnce then
		l_Player.m_AnimationTime = 0
		l_Player.m_CameraController:unlock()
		l_Owner:remove_action(l_Owner:get_actual_action_animation())
		l_Player:ClearCamera()
	end
	if l_Player.m_Teleport then
		l_Player.m_Teleport = false
		ChangeLevel(args)
	end
	
end

function InteractingToIdleCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting and not l_Player.m_SingOnce
end

function InteractingToSpecialSingingCondition(args)
	local l_Player = args["self"]
	return not l_Player.m_IsInteracting and l_Player.m_SingOnce
end
