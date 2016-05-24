function IdleFirstAutomaton(args)
	utils_log("IdleFirst")
end

function IdleUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	
	l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Owner:get_position())
	
	if l_Distance < 10.0 then
		l_Enemy.m_Chase = true
	end
end

function IdleEndAutomaton(args)
	utils_log("IdleEnd")
end

function IdleToPatrolConditionAutomaton()
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	return l_Enemy.m_Patrol
end

function IdleToChaseConditionAutomaton()
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	return l_Enemy.m_Chase
end