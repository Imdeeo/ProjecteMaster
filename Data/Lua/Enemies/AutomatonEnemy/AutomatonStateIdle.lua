function IdleFirstAutomaton(args)
end

function IdleUpdateAutomaton(args, _ElapsedTime)

end

function IdleEndAutomaton(args)
end

function IdleToPatrolConditionAutomaton()
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	return l_Enemy.m_Patrol
end

function IdleToChaseConditionAutomaton()
	local l_Enemy = m_CharacterManager.m_Enemics[1]	
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	return l_Distance < l_Enemy.m_distance_to_activate
end