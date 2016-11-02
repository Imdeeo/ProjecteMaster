function R2PushOrganKey(_Key, _Player)
	if _Player.m_OrganKeyOrder[_Player.m_OrganKeyCount] == _Key then
	utils_log("Correct Key")
		_Player.m_OrganKeyCount = _Player.m_OrganKeyCount + 1
	elseif _Player.m_OrganKeyOrder[1] == _Key then
	utils_log("Incorrect Key, but is first")
		_Player.m_OrganKeyCount = 2
	else
	utils_log("Incorrect Key, resetting")
		_Player.m_OrganKeyCount = 1
	end

	if _Player.m_OrganKeyCount > table_length(_Player.m_OrganKeyOrder) then
		g_TimerManager:ExecuteLater(3.0, function()
			utils_log("Puzzle cleared")
			_Player.m_IsPuzzle = false
			_Player.m_InteractingAnimation = 9
			_Player.m_CameraAnimation = "PipeOrganEnd"
			_Player.m_CinematicManager:get_resource("ResolvePuzzle"):play()
			_Player.m_SoundManager:set_rtpc_value(g_InitialDelayRTPC, 0, "SecretDoor")
			_Player.m_SoundManager:play_event(g_BookShelfSoundEvent, "SecretDoor")
			_Player.m_PhysXManager:enable_object("puertalaberintooficialmovida", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("puertalaberintooficial", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyA", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyB", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyC", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyD", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyE", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyF", "FisicasAux")
			_Player.m_PhysXManager:disable_physics("TriggerOrganKeyG", "FisicasAux")
		end)
	end
end
