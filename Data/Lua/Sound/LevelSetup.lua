local l_LevelLoadFunctions = {}

l_LevelLoadFunctions['Player'] = function(_SoundManager)

end

l_LevelLoadFunctions['Recibidor'] = function(_SoundManager)

end

l_LevelLoadFunctions['Biblioteca'] = function(_SoundManager)
	_SoundManager:play_event(g_FireSoundEvent, "Fireplace")
	_SoundManager:play_event(g_NewcomenSoundEvent, "SpeakerNewcomenTable")
	_SoundManager:play_event(g_NewcomenSoundEvent, "SpeakerNewcomenOrgan")
end

l_LevelLoadFunctions['Maquinas'] = function(_SoundManager)
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltRight")
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltCenter")
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltLeft")
	_SoundManager:play_event(g_CauldronSoundEvent, "SpeakerCauldronTop")
	_SoundManager:play_event(g_CauldronSoundEvent, "SpeakerCauldronBot")
	_SoundManager:play_event(g_SteamSoundEvent, "SpeakerInputPipe")
end

l_LevelLoadFunctions['Pasillo'] = function(_SoundManager)
	_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerWheels1")
	_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerWheels2")
	_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerWheels3")
	_SoundManager:play_event(g_CogwheelsSoundEvent, "SpeakerWheels4")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail1")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail3RightRight")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail3LeftLeft")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail3LeftRight")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail3RightLeft")
	_SoundManager:play_event(g_JailSoundEvent, "SpeakerJail4")
	_SoundManager:play_event(g_CaveSoundEvent, "SpeakerCave")
end

l_LevelLoadFunctions['Boss'] = function(_SoundManager)
	_SoundManager:play_event(g_HijoVoiceEvent)
end

function SetupLevelSounds(_LevelName, _SoundManager)
	l_LevelLoadFunctions[_LevelName](_SoundManager)
end



local l_SpeakersToSilence = {}

l_SpeakersToSilence['Player'] = {
}

l_SpeakersToSilence['Recibidor'] = {
	'Cogwheel1',
	'Cogwheel2',
	'Cogwheel3',
	'SpeakerWheelsRight',
	'SpeakerWheelsTop',
	'SpeakerWheelLeftRight',
	'SpeakerWheelLeftLeft',
}
l_SpeakersToSilence['Biblioteca'] = {
	'Fireplace',
	'Projector',
	'SpeakerFire',
	'SpeakerNewcomenTable',
	'SpeakerNewcomenOrgan',
	'SpeakerProjector',
}
l_SpeakersToSilence['Maquinas'] = {
	'SpeakerTesla',
	'SpeakerNewcomen',
	'SpeakerLeverWheelLeft',
	'SpeakerInputPipe',
	'SpeakerDoorPipe',
	'SpeakerCauldronTop',
	'SpeakerCauldronBot',
	'SpeakerCylindersLeft',
	'SpeakerCylindersRight',
	'SpeakerLeverWheelRight',
	'SpeakerHourglass',
	'SpeakerToothWheelRight',
	'SpeakerToothWheelLeft',
	'SpeakerCrusherWall',
	'SpeakerCrusherCeiling',
	'SpeakerBeltCenter',
	'SpeakerBeltLeft',
	'SpeakerBeltRight',
	'SpeakerPipeDoor',
}

l_SpeakersToSilence['Boss'] = {
}

l_SpeakersToSilence['Pasillo'] = {
}

function SilenceLevelSounds(_LevelName)
	for i, l_SpeakerName in ipairs(l_SpeakersToSilence[_LevelName]) do
		g_SoundManager:play_event(g_SilenceSpeakerSoundEvent, l_SpeakerName)
	end
end
