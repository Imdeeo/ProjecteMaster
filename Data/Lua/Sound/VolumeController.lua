class "VolumeController"
  function VolumeController:__init()
    self.m_MusicVolume = 50.0
    self.m_FXVolume = 50.0

    self.m_MusicRTPC = SoundRTPC()
    self.m_MusicRTPC.rtpc_name = "music_volume"
    self.m_FXRTPC = SoundRTPC()
    self.m_FXRTPC.rtpc_name = "fx_volume"

    self.m_InputManager = CInputManager.get_input_manager()
    self.m_SoundManager = CUABEngine.get_instance():get_sound_manager()
  end

  function VolumeController:CheckVolumeKeys()
    if self.m_InputManager:is_action_active("MUSIC_VOLUME_UP") then
      self:SetMusicVolume(self.m_MusicVolume + 5.0)
    end
    if self.m_InputManager:is_action_active("MUSIC_VOLUME_DOWN") then
      self:SetMusicVolume(self.m_MusicVolume - 5.0)
    end
    if self.m_InputManager:is_action_active("FX_VOLUME_UP") then
      self:SetFXVolume(self.m_FXVolume + 5.0)
    end
    if self.m_InputManager:is_action_active("FX_VOLUME_DOWN") then
      self:SetFXVolume(self.m_FXVolume - 5.0)
    end
  end

  function VolumeController:SetMusicVolume(newVolume) -- volume values between 0 and 100
    self.m_MusicVolume = newVolume
    self.m_SoundManager:set_rtpc_value(self.m_MusicRTPC, self.m_MusicVolume, "NO_SPEAKER")
  end

  function VolumeController:SetFXVolume(newVolume)
    self.m_FXVolume = newVolume
    self.m_SoundManager:set_rtpc_value(self.m_FXRTPC, self.m_FXVolume, "NO_SPEAKER")
  end
