g_SoundManager = CUABEngine.get_instance():get_sound_manager()

--// Recibidor
function PlayOpenDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.1667, _Speaker)
	g_SoundManager:play_event(g_OpenDrawerSoundEvent, _Speaker)
end

function PlayCloseDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.5, _Speaker)
	g_SoundManager:play_event(g_CloseDrawerSoundEvent, _Speaker)
end

function PlayForceDrawerSound(_Speaker)

	g_SoundManager:set_rtpc_value(g_PitchRTPC, -80, "Cogwheel1")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 4.1, "Cogwheel1")
	g_SoundManager:play_event(g_CogwheelsSoundEvent, "Cogwheel1")
	g_SoundManager:set_rtpc_value(g_PitchRTPC, 0, "Cogwheel2")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 3, "Cogwheel2")
	g_SoundManager:play_event(g_CogwheelsSoundEvent, "Cogwheel2")
	g_SoundManager:set_rtpc_value(g_PitchRTPC, 30, "Cogwheel3")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 3.4, "Cogwheel3")
	g_SoundManager:play_event(g_CogwheelsSoundEvent, "Cogwheel3")

	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.2, _Speaker)
	g_SoundManager:set_rtpc_value(g_SecondaryDelayRTPC, 4.0, _Speaker)
	g_SoundManager:play_event(g_ForceDrawerSoundEvent, _Speaker)
end

--// Biblioteca
function PlayEnterLibrarySound(_Speaker)
	g_SoundManager:play_event(g_EnterLibrarySondEvent, _Speaker)
end

function PlayPickupClueSounds(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.2, _Speaker)
	g_SoundManager:set_rtpc_value(g_SecondaryDelayRTPC, 6.5, _Speaker)
	g_SoundManager:play_event(g_ClueSheetPickupSoundEvent, _Speaker)
end

function PlayActivateProjectorSounds(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.3, _Speaker)
	g_SoundManager:play_event(g_BookSwitchSoundEvent, _Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 2.2, "Projector")
	g_SoundManager:play_event(g_ProjectorSoundEvent, "Projector")
end

function PlayOpenSecretDoorSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0.5, _Speaker)
	g_SoundManager:play_event(g_OpenSecretDoorSoundEvent, _Speaker)
end

function PlayPickupArtifactSounds(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.1, _Speaker)
	g_SoundManager:play_event(g_ArtifactPickupSoundEvent, _Speaker)
end

function PlayOpenArtifactDoorSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, _Speaker)
	g_SoundManager:play_event(g_ArtifactUseSoundEvent, _Speaker)
end

 --// Maquinas
function PlayPickValveSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 2.6667, _Speaker)
	g_SoundManager:play_event(g_PickValveEvent, _Speaker)
end

function PlayInputValveSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.1667, _Speaker)
	g_SoundManager:play_event(g_InputValveEvent, _Speaker)
end

function PlayOpenValveDoorSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1, _Speaker)
	g_SoundManager:set_rtpc_value(g_SecondaryDelayRTPC, 1.3333, _Speaker)
	g_SoundManager:play_event(g_OpenValveDoor, _Speaker)
end

function PlayStairsStartSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, _Speaker)
	g_SoundManager:play_event(g_ClimbStartSoundEvent, _Speaker)
end

function PlayStairsLoopSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, _Speaker)
	g_SoundManager:play_event(g_ClimbLoopSoundEvent, _Speaker)
end

function PlayStairsEndSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, _Speaker)
	g_SoundManager:play_event(g_ClimbEndSoundEvent, _Speaker)
end

function PlayRunSound(_Speaker)

end

function PlayNullSound(_Speaker)

end

-- The order must correspond to the numbers of animations.
g_InteractionSounds = {
	PlayEnterLibrarySound,
	PlayOpenDrawerSound,
	PlayCloseDrawerSound,
	PlayForceDrawerSound,
	PlayOpenArtifactDoorSound,
	PlayPickupArtifactSounds,
	PlayPickupClueSounds,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayActivateProjectorSounds,
	PlayStairsStartSound,
	PlayStairsLoopSound,
	PlayStairsLoopSound,
	PlayNullSound,
	PlayStairsEndSound,
	PlayNullSound,
	PlayPickValveSound,
	PlayInputValveSound,
	PlayOpenValveDoorSound,
	PlayRunSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound,
	PlayNullSound
}

