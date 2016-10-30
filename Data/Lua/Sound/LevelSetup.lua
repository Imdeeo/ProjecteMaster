function SetupSoundsBiblioteca(_SoundManager)
	local l_FireSoundEvent = SoundEvent()
	l_FireSoundEvent.event_name = "Fire_Start"

	_SoundManager:play_event(l_FireSoundEvent, "Fireplace")
end
