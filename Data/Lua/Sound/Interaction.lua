g_SoundManager = CUABEngine.get_instance():get_sound_manager()

function PlayEnterLibrarySound(_Speaker)
	g_SoundManager:play_event(g_EnterLibrarySondEvent, _Speaker)
end

function PlayOpenDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0.6, _Speaker)
	g_SoundManager:play_event(g_OpenDrawerSoundEvent, _Speaker)
end

function PlayCloseDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.1, _Speaker)
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

function PlayOpenArtifactDoorSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, _Speaker)
	g_SoundManager:play_event(g_ArtifactUseSoundEvent, _Speaker)
end

function PlayPickupArtifactSounds(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.1, _Speaker)
	g_SoundManager:play_event(g_ArtifactPickupSoundEvent, _Speaker)
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

-- The order must correspond to the numbers of animations.
g_InteractionSounds = {
	PlayEnterLibrarySound,
	PlayOpenDrawerSound,
	PlayCloseDrawerSound,
	PlayForceDrawerSound,
	PlayOpenArtifactDoorSound,
	PlayPickupArtifactSounds,
	PlayPickupClueSounds,
}
g_InteractionSounds[17] = PlayActivateProjectorSounds

