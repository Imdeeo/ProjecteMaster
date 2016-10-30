g_SoundManager = CUABEngine.get_instance():get_sound_manager()

g_OpenDrawerSoundEvent = SoundEvent()
g_OpenDrawerSoundEvent.event_name = "Drawer_Open"
g_CloseDrawerSoundEvent = SoundEvent()
g_CloseDrawerSoundEvent.event_name = "Drawer_Close"
g_ForceDrawerSoundEvent = SoundEvent()
g_ForceDrawerSoundEvent.event_name = "Drawer_Force"

g_InitialDelayRTPC = SoundRTPC()
g_InitialDelayRTPC.rtpc_name = "initial_delay"
g_ObjectPickupDelayRTPC = SoundRTPC()
g_ObjectPickupDelayRTPC.rtpc_name = "pick_object_delay"

function PlayOpenDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0.7, _Speaker)
	g_SoundManager:play_event(g_OpenDrawerSoundEvent, _Speaker)
end

function PlayCloseDrawerSound(_Speaker)
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.0, _Speaker)
	g_SoundManager:play_event(g_CloseDrawerSoundEvent, _Speaker)
end

function PlayForceDrawerSound(_Speaker)
	local l_CogwheelsSoundEvent = SoundEvent()
	l_CogwheelsSoundEvent.event_name = "Cogwheels_Start"
	local l_PitchRTPC = SoundRTPC()
	l_PitchRTPC.rtpc_name = "pitch_difference"

	g_SoundManager:set_rtpc_value(l_PitchRTPC, -80, "Cogwheel1")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 4.1, "Cogwheel1")
	g_SoundManager:play_event(l_CogwheelsSoundEvent, "Cogwheel1")
	g_SoundManager:set_rtpc_value(l_PitchRTPC, 0, "Cogwheel2")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 3, "Cogwheel2")
	g_SoundManager:play_event(l_CogwheelsSoundEvent, "Cogwheel2")
	g_SoundManager:set_rtpc_value(l_PitchRTPC, 30, "Cogwheel3")
	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 3.4, "Cogwheel3")
	g_SoundManager:play_event(l_CogwheelsSoundEvent, "Cogwheel3")

	g_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 1.2, _Speaker)
	g_SoundManager:set_rtpc_value(g_ObjectPickupDelayRTPC, 4.0, _Speaker)
	g_SoundManager:play_event(g_ForceDrawerSoundEvent, _Speaker)
	--[[
	local l_LayerManager = CUABEngine.get_instance():get_level_manager():get_level("Recibidor"):get_layer_manager()
	local l_SolidLayer = l_LayerManager:get_layer("solid")
	local l_AnimationSoundEvent = self.m_SoundEvents[self.m_InteractingAnimation]
	local l_Speaker = l_SolidLayer:get_resource("CajonComodaDerecha1")
	if l_AnimationSoundEvent ~= nil and self.m_InteractionSoundSpeaker ~= nil then
		self.m_SoundManager:play_event(l_AnimationSoundEvent, l_Speaker)
	end]]
end

g_InteractingSounds = {
	nil,
	PlayOpenDrawerSound,
	PlayCloseDrawerSound,
	PlayForceDrawerSound,
}
--g_InteractingSounds[2] = PlayOpenDrawerSound

