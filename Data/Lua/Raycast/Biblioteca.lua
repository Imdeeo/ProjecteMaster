dofile("Data\\Lua\\Raycast\\PipeOrgan.lua")
dofile("Data\\Lua\\Player\\Helpers.lua")

local l_LevelId = "Biblioteca"

function R2TriggerOrganKeyA(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla1")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(10, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("A", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteC, "SpeakerOrganLeft") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganLeft") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganLeft") 
	end
end

function R2TriggerOrganKeyB(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla2")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(11, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("B", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteD, "SpeakerOrganLeft") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganLeft") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganLeft") 
	end
end

function R2TriggerOrganKeyC(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla3")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(12, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("C", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteE, "SpeakerOrganCenter") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganCenter") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganCenter")
	end
end

function R2TriggerOrganKeyD(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla4")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(13, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("D", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteF, "SpeakerOrganCenter") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganCenter") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganCenter") 
	end
end

function R2TriggerOrganKeyE(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla5")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(14, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("E", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteG, "SpeakerOrganCenter") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganCenter") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganCenter")
	end
end

function R2TriggerOrganKeyF(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla6")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(15, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("F", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteA, "SpeakerOrganRight") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganRight") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganRight") 
	end
end

function R2TriggerOrganKeyG(_Player, _Owner)
	local l_cinematic = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_cinematic_manager():get_resource("Tecla7")
	if l_cinematic:get_current_time() == 0 then
		_Owner:remove_action(_Owner:get_actual_action_animation())
		_Owner:execute_action(16, 0.1, 0.1, 1.0, true)
		l_cinematic:play()
		R2PushOrganKey("G", _Player)
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:set_switch(g_OrganNoteB, "SpeakerOrganRight") 
		l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.4, "SpeakerOrganRight") 
		l_SoundManager:play_event(g_OrganPlayNoteEvent, "SpeakerOrganRight")
	end
end

function R2TriggerPipeOrgan(_Player)
	_Player.m_TargetYaw = -g_PI/2.0
	_Player.m_TargetPosOffset = Vect3f(0.017, 0.0, -0.75)
	_Player.m_ForwardCamera = Vect3f(0.0, -0.32014, 0.94737)
	_Player.m_UpCamera = Vect3f(0.0, 0.947369, 0.320145)
	l_Target = GetTriggerPos("TriggerPipeOrgan",l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.8) then
		_Player.m_Target = Vect3f(0.124282, 1.4032, 14.3227)
		_Player.m_AnimationTime = 0.6667
		_Player.m_InteractingAnimation = 8
		_Player.m_InteractingCinematic = nil
		_Player.m_CameraAnimation = "PipeOrganStart"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = true
		_Player.m_PhysXManager:disable_physics("TriggerPipeOrgan", "FisicasAux")
	end
end

function R2Artifact(_Player)
	_Player.m_TargetYaw = -g_PI/2.0
	_Player.m_TargetPosOffset = Vect3f(0.019, 0.0, -0.57)
	_Player.m_ForwardCamera = Vect3f(0.0, 0.0, 1.0)
	_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
	l_Target = GetTriggerPos("TriggerArtifact",l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.8) then
		_Player.m_Target = Vect3f(-17.0552, 1.66966, 28.9583)
		_Player.m_AnimationTime = 5
		_Player.m_LeftHanded = true
		_Player.m_InteractingAnimation = 6
		_Player.m_InteractingCinematic = nil
		_Player.m_CameraAnimation = "PickArtifact"
		_Player.m_NewItemName = "Artilufacto"
		_Player.m_ItemTime = 1.01
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_BiasPos = 0.03
		_Player.m_PhysXManager:disable_physics("TriggerArtifact", "FisicasAux")
		_Player.m_InteractionSoundSpeaker = "SpeakerArtifactStand"
	end
end

function R2ArtifactDoor(_Player)
	_Player.m_TargetYaw = -g_PI/2.0
	_Player.m_TargetPosOffset = Vect3f(-0.770, 0.0, -0.756)
	_Player.m_ForwardCamera = Vect3f(0.0, -0.67204, 1.0)
	_Player.m_UpCamera = Vect3f(0.0, 0.740519, 0.672036)
	l_Target = GetTriggerPos("TriggerArtifactDoor", l_LevelId) 
	if _Player:IsFacingTarget(l_Target, 1.0, 1.6) then
		_Player.m_Target = Vect3f(17.3453, 5.35695, 27.0514)
		_Player.m_InteractingAnimation = 5
		_Player.m_InteractingCinematic = "CrossArtifactDoor"
		_Player.m_CameraAnimation = "CrossArtifactDoor"
		_Player.m_LeftHanded = false
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.6667
		_Player.m_ItemDropTime = -1.0
		--_Player.m_CurrentAend = "CrossArtifactDoor"
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_Teleport = true
		_Player.m_IsPuzzle = false
		_Player.m_InteractionSoundSpeaker = "SpeakerArtifactDoor"
	end
end

function R2Clue(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_TargetPosOffset = Vect3f(-0.646, 0.0, 0.03)
	_Player.m_ForwardCamera = Vect3f(0.776573, -0.63003, 0.0)
	_Player.m_UpCamera = Vect3f(0.630027, 0.776573, 0.0)
	l_Target = GetTriggerPos("TriggerClue",l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.6) then
		_Player.m_Target = Vect3f(11.8743, 1.2716, 15.2575)
		_Player.m_InteractingAnimation = 7
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 7
		_Player.m_CameraAnimation = "PickClue"
		_Player.m_NewItemName = "PistaBiblio"
		_Player.m_LeftHanded = true
		_Player.m_ItemTime = 1
		_Player.m_ItemDropTime = 6.5
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_InteractionSoundSpeaker = "SpeakerClues"
	end
end

function R2Book(_Player)
	_Player.m_TargetYaw = -g_PI/2.0
	_Player.m_TargetPosOffset = Vect3f(0.03, 0.0, -0.62)
	_Player.m_ForwardCamera = Vect3f(0.0, 0.0, 1.0)
	_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
	l_Target = GetTriggerPos("TriggerBook",l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.8) then
		_Player.m_Target = Vect3f(15.5708, 1.52933, 26.3092)
		_Player.m_InteractingAnimation = 17
		_Player.m_InteractingCinematic = "PullBook"
		_Player.m_CameraAnimation = "PullBook"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		g_Engine:get_video_manager():play_clip("puzzlebiblio.ogv") -- launch projector video
		_Player.m_CinematicManager:get_resource("fonotelefote"):play() -- launch projector cinematic
		local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
		l_Level:get_light_manager():get_resource("LuzProyector"):set_enabled(true) -- activate lights
		l_Level:get_layer_manager():get_layer("alpha_blend_objects"):get_resource("reflejoluz3"):set_visible(true) 
		-- activate particles
		_Player.m_PhysXManager:disable_physics("TriggerBook", "FisicasAux")
		_Player.m_InteractionSoundSpeaker = "SpeakerBook"
	end
end
