local l_LevelLoadFunctions = {}

l_LevelLoadFunctions['Recibidor'] = function(_SoundManager)
end

l_LevelLoadFunctions['Biblioteca'] = function(_SoundManager)
	_SoundManager:play_event(g_FireSoundEvent, "Fireplace")
end

l_LevelLoadFunctions['Maquinas'] = function(_SoundManager)
end

l_LevelLoadFunctions['Pasillo'] = function(_SoundManager)
end

l_LevelLoadFunctions['Boss'] = function(_SoundManager)
end


function SetupLevelSounds(_LevelName, _SoundManager)
	l_LevelLoadFunctions[_LevelName](_SoundManager)
end
