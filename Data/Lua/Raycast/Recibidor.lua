dofile("Data\\Lua\\Player\\Helpers.lua")

R1TrayR1IsClosed = true
R1TrayR2IsClosed = true
R1TrayL1IsClosed = true
R1TrayL2IsClosed = true

local l_LevelId = "Recibidor"

function RegisterSpeakersRecibidor()
	local l_Engine = CUABEngine.get_instance()
	local l_SoundManager = l_Engine:get_sound_manager()
	local l_LevelManager = l_Engine:get_level_manager()
	local l_Level = l_LevelManager:get_level("Recibidor")
	local l_LayerManager = l_Level:get_layer_manager()
	local l_SolidLayer = l_LayerManager:get_layer("solid")

	
	l_SoundManager:register_speaker(l_SolidLayer:get_resource("CajonComodaDerecha1"))
	l_SoundManager:register_speaker(l_SolidLayer:get_resource("CajonComodaDerecha2"))
end

function R1Door(_Player)
	_Player.m_TargetPosOffset = Vect3f(-1.377, 0.0, -0.713147)
	_Player.m_ForwardCamera = Vect3f(0.0, -0.5, 0.86)
	_Player.m_UpCamera = Vect3f(0.0, 0.866026, 0.5)
	_Player.m_TargetYaw = -g_PI/2.0
	l_Target = GetTriggerPos("TriggerDoor", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(0.884729, 0.0, 5.0229)
		_Player.m_InteractingAnimation = 1
		_Player.m_InteractingCinematic = "CrossDoor"
		_Player.m_CameraAnimation = "CrossDoor"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.5
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = false
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_Teleport = true
		_Player.m_SingOnce = true
		_Player.m_AnimationTime = 16.6
		local l_LevelManager = g_Engine:get_level_manager()
		l_LevelManager:get_level("Biblioteca"):set_visible(true)		
	end
end

function R1Sheets(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_TargetPosOffset = Vect3f(0.5, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerSheets", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.0) then
		_Player.m_Target = l_Target
		-- Play Sound
		--utils_log("<-Interacted with R1Sheets->")
	end
end

function R1Canvas(_Player)
	_Player.m_TargetYaw = 0.0
	_Player.m_TargetPosOffset = Vect3f(-1.5, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerCanvas", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 2.0) then
		_Player.m_Target = l_Target
		-- Play Sound
		--utils_log("<-Interacted with R1Canvas->")
	end
end

function R1TrayR1(_Player) --This contains the key
	_Player.m_TargetYaw = 0.0
	--utils_log("Yaw: ".._Player.m_TargetYaw)
	_Player.m_UpCamera = Vect3f(-0.682049, 0.731307, 0.0)	
	_Player.m_ForwardCamera = Vect3f(-0.7313, -0.68205, 0)
	_Player.m_TargetPosOffset = Vect3f(0.998, 0, 0.025)
	l_Target = GetTriggerPos("TriggerTrayR1", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(-2.423140, 1.000280, -2.373670)
		_Player.m_InteractingAnimation = 4
		_Player.m_InteractingCinematic = "ForceTray"
		_Player.m_CameraAnimation = "ForceTray"
		_Player.m_NewItemName = "LlaveRecibidor"
		_Player.m_ItemTime = 4.06666
		_Player.m_ItemDropTime = -1.0
		_Player.m_LeftHanded = true
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		g_TimerManager:ExecuteLater(3.5, function()
			m_CharacterManager.m_EnemicsMap["Recibidor"]["CagedAutomatonLeft"].m_Awake = true
		end)
		g_TimerManager:ExecuteLater(4.0, function()
			m_CharacterManager.m_EnemicsMap["Recibidor"]["CagedAutomatonRight"].m_Awake = true
		end)
		_Player.m_PhysXManager:disable_physics("TriggerTrayR1", "FisicasAux")
		
		local l_LevelManager = g_Engine:get_level_manager()
		l_LevelManager:get_level(l_LevelId):get_cinematic_manager():get_resource("recibidor_engranajes"):play()
	end
	_Player.m_InteractionSoundSpeaker = "CajonComodaDerecha1"
end

function R1TrayR2Open(_Player)
	_Player.m_UpCamera = Vect3f(-0.69189, 0.722003, 0.0)	
	_Player.m_ForwardCamera = Vect3f(-0.722003, -0.691890, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.974, 0, 0.025)
	_Player.m_TargetYaw = 0.0
	l_Target = GetTriggerPos("TriggerTrayR2", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(-2.423140, 1.000280, -1.056070)
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayR2"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayR2IsClosed = false
	end
end

function R1TrayR2Close(_Player)
	_Player.m_TargetYaw = 0.0
	_Player.m_UpCamera = Vect3f(-0.772938, 0.634248, 0.0172184)	
	_Player.m_ForwardCamera = Vect3f(-0.63435, -0.773049, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.824, 0, 0.00)
	l_Target = GetTriggerPos("TriggerTrayR2", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(-2.423140, 1.000280, -1.056070)
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayR2"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayR2IsClosed = true
	end
end

function R1TrayR2(_Player)
	if R1TrayR2IsClosed then
		R1TrayR2Open(_Player)
	else
		R1TrayR2Close(_Player)
	end
	_Player.m_InteractionSoundSpeaker = "CajonComodaDerecha2"
end

function R1TrayL1Open(_Player)
	_Player.m_UpCamera = Vect3f(0.69189, 0.722003, 0.0)
	_Player.m_ForwardCamera = Vect3f(0.722003, -0.691890, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.974, 0, 0.00)
	_Player.m_TargetYaw = g_PI
	l_Target = GetTriggerPos("TriggerTrayL1", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(2.420210, 1.000280, -2.373670)
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL1"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_SingOnce = false
		R1TrayL1IsClosed = false
	end
end

function R1TrayL1Close(_Player)
	_Player.m_UpCamera = Vect3f(0.772938, 0.634248, 0.0172184)	
	_Player.m_ForwardCamera = Vect3f(0.63435, -0.773049, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.874, 0, 0.0)
	_Player.m_TargetYaw = g_PI
	l_Target = GetTriggerPos("TriggerTrayL1", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		_Player.m_Target = Vect3f(2.420210, 1.000280, -2.373670)
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL1"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayL1IsClosed = true
	end
end

function R1TrayL1(_Player)
	if R1TrayL1IsClosed then
		R1TrayL1Open(_Player)
	else
		R1TrayL1Close(_Player)
	end
	_Player.m_InteractionSoundSpeaker = "CajonComodaDerecha2"
end

function R1TrayL2Open(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_UpCamera = Vect3f(0.69189, 0.722003, 0.0)	
	_Player.m_ForwardCamera = Vect3f(0.722003, -0.691890, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.974, 0, 0.00)
	l_Target = GetTriggerPos("TriggerTrayL2", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		local l_Level = g_Engine:get_level_manager():get_level(l_LevelId)
		l_Level:get_layer_manager():get_layer("triggers"):get_resource("TriggerTrayL2"):set_scale(Vect3f(1.0, 0.5, 1.0))
		_Player.m_Target = Vect3f(2.408430, 1.000280, -1.056070)
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL2"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_SingOnce = false
		R1TrayL2IsClosed = false
	end
end

function R1TrayL2Close(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_UpCamera = Vect3f(0.772938, 0.634248, 0.0172184)	
	_Player.m_ForwardCamera = Vect3f(0.63435, -0.773049, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.824, 0, 0.0)
	l_Target = GetTriggerPos("TriggerTrayL2", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 0.8) then
		local l_Level = g_Engine:get_level_manager():get_level(l_LevelId)
		l_Level:get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL2"):set_scale(Vect3f(1.0, 1.0, 1.0))
		_Player.m_Target = Vect3f(2.408430, 1.000280, -1.056070)
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL2"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayL2IsClosed = true
	end
end

function R1TrayL2(_Player) --This contains the ankh
	if R1TrayL2IsClosed then
		R1TrayL2Open(_Player)
	else
		R1TrayL2Close(_Player)
	end
	_Player.m_InteractionSoundSpeaker = "CajonComodaDerecha2"
end

function R1Ankh(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_TargetPosOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerAnkh",l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.0) then
		_Player.m_Target = l_Target
		-- Play Sound
	end
end
