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
	utils_log("Puzzle cleared")
		_Player.m_IsPuzzle = false
		_Player.m_InteractingAnimation = 9
		_Player.m_CameraAnimation = "PipeOrganEnd"
		_Player.m_CinematicManager:get_resource("ResolvePuzzle"):play()
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyA")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyB")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyC")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyD")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyE")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyF")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyG")
	end
end
