local l_LevelLoadFunctions = {}

l_LevelLoadFunctions['Recibidor'] = function(_SoundManager)

end

l_LevelLoadFunctions['Biblioteca'] = function(_SoundManager)
	_SoundManager:play_event(g_FireSoundEvent, "Fireplace")
	_SoundManager:play_event(g_NewcomenSoundEvent, "SpeakerNewcomenTable")
	_SoundManager:play_event(g_NewcomenSoundEvent, "SpeakerNewcomenOrgan")
end

l_LevelLoadFunctions['Maquinas'] = function(_SoundManager)
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltLeft")
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltCenter")
	_SoundManager:play_event(g_ConveyorBeltSoundEvent, "SpeakerBeltRight")
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
