function R2PushOrganKey(_Key, _Player)
	if _Player.m_OrganKeyOrder[_Player.m_OrganKeyCount] == _Key then
		_Player.m_OrganKeyCount = m_OrganKeyCount + 1
	elseif _Player.m_OrganKeyOrder[1] == _Key then
		_Player.m_OrganKeyCount = 2
	else
		_Player.m_OrganKeyCount = 1
	end
	
	if m_OrganKeyCount > m_OrganKeyOrder:size() then
		_Player.m_IsPuzzle = false
		_Player.m_InteractingAnimation = 9
		_Player.m_CameraAnimation = "PipeOrganEnd"
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyA")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyB")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyC")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyD")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyE")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyF")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyG")
	end
end
