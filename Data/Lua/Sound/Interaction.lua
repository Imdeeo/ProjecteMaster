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
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 2.2, "SpeakerProjector")
	g_SoundManager:play_event(g_ProjectorSoundEvent, "SpeakerProjector")
	g_TimerManager:ExecuteLater(2.2, function()
		g_SoundManager:play_event(g_PuzzleVideoSoundEvent)
		g_SoundManager:broadcast_state(g_PuzzleVideoActionState)
	end)
	g_TimerManager:ExecuteLater(21.0, function()
		g_SoundManager:broadcast_state(g_ExplorationActionState)
	end)
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
	g_TimerManager:ExecuteLater(6.0, function()
		g_SoundManager:play_event(g_SilenceSpeakerSoundEvent, "SpeakerInputPipe")
	end)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 4.4, "SpeakerCrusherWall")
	g_SoundManager:play_event(g_CrusherSoundEvent, "SpeakerCrusherWall")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 4.9, "SpeakerCrusherCeiling")
	g_SoundManager:play_event(g_CrusherSoundEvent, "SpeakerCrusherCeiling")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 5.6, "SpeakerToothWheelRight")
	g_SoundManager:play_event(g_MiniwheelSoundEvent, "SpeakerToothWheelRight")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 5.6, "SpeakerToothWheelLeft")
	g_SoundManager:play_event(g_MiniwheelSoundEvent, "SpeakerToothWheelLeft")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 5.9, "SpeakerHourglass")
	g_SoundManager:play_event(g_HourglassSoundEvent, "SpeakerHourglass")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 6.3, "SpeakerCylindersRight")
	g_SoundManager:play_event(g_CylinderSoundEvent, "SpeakerCylindersRight")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 6.3, "SpeakerCylindersLeft")
	g_SoundManager:play_event(g_CylinderSoundEvent, "SpeakerCylindersLeft")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 6.3, "SpeakerLeverWheelRight")
	g_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerLeverWheelRight")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 6.9, "SpeakerLeverWheelLeft")
	g_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerLeverWheelLeft")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 7.3, "SpeakerNewcomen")
	g_SoundManager:play_event(g_NewcomenSoundEvent, "SpeakerNewcomen")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 7.8, "SpeakerTesla")
	g_SoundManager:play_event(g_TeslaSoundEvent, "SpeakerTesla")
	g_SoundManager:set_rtpc_value(g_PitchRTPC, -80, "SpeakerDoorPipe")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 8.0, "SpeakerDoorPipe")
	g_SoundManager:play_event(g_SteamSoundEvent, "SpeakerDoorPipe")
	
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

