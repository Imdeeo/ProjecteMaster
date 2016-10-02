m_OrganLastKey = nil
m_OrganNewKey = nil
m_OrganKeyCount = 1
m_OrganKeyOrder = {"A", "B", "C", "D", "E", "F"}

function R2PushOrganKey(_Key, _Player)
	if m_OrganKeyOrder[m_OrganKeyCount] == _Key then
		m_OrganKeyCount = m_OrganKeyCount + 1
	else
		m_OrganKeyCount = 2
	end
	m_OrganLastKey = m_OrganNewKey
	m_OrganNewKey = _Key
	
	if m_OrganKeyCount > m_OrganKeyOrder:size() then
		_Player.m_CinematicManager:get_resource("ResolvePuzzle"):play()
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyA")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyB")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyC")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyD")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyE")
		_Player.m_PhysXManager:disable_trigger("TriggerOrganKeyF")
	end
end
