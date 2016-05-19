g_MusicVolume = 50.0
g_FXVolume = 50.0

g_MusicRTPC = SoundRTPC()
g_MusicRTPC.rtpc_name = "main_bus_volume"
g_FXRTPC = SoundRTPC()
g_FXRTPC.rtpc_name = "fx_volume"

function UpdateVolume()
  local l_InputManager = CInputManager.get_input_manager()
  local l_SoundManager = CUABEngine.get_instance():get_sound_manager()
  if l_InputManager:is_action_active("VOLUME_UP") then
    g_MusicVolume = g_MusicVolume + 5.0
    g_FXVolume = g_FXVolume + 5.0
    utils_log("music volume: "..g_MusicVolume)
    --utils_log("fx volume: "..g_FXVolume)
    l_SoundManager:set_rtpc_value(g_MusicRTPC, g_MusicVolume, "NO_SPEAKER")
    --l_SoundManager:broadcast_rtpc_value(g_FXRTPC, g_FXVolume)
  end
  if l_InputManager:is_action_active("VOLUME_DOWN") then
    g_MusicVolume = g_MusicVolume - 5.0
    g_FXVolume = g_FXVolume - 5.0
    utils_log("music volume: "..g_MusicVolume)
    --utils_log("fx volume: "..g_FXVolume)
    l_SoundManager:set_rtpc_value(g_MusicRTPC, g_MusicVolume, "NO_SPEAKER")
    --l_SoundManager:broadcast_rtpc_value(g_FXRTPC, g_FXVolume)
  end
end
