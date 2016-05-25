function OffFirstAutomaton(args)
end

function OffUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Owner:get_position())
	
	if l_Distance < l_Enemy.m_distance_to_activate then
		l_Enemy.m_Activate = true
	end
end

function OffEndAutomaton(args)
end

function OffToIdleConditionAutomaton()	
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	return l_Enemy.m_Activate
end