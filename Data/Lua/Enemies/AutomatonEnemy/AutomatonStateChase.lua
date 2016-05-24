function ChaseFirstAutomaton(args)
	utils_log("ChaseFirst")
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	
	l_Owner:blend_cycle(0,1.0,0.1)
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Enemy.m_RenderableObject:get_position()
	local l_Direction = (l_PlayerPos - l_EnemyPos)
		
	l_Owner:set_position(l_EnemyPos + (l_Direction * l_Enemy.m_Speed * _ElapsedTime))
end

function ChaseEndAutomaton(args)
	utils_log("ChaseEnd")
end

function ChaseToAlertConditionAutomaton()
end

function ChaseToAttackConditionAutomaton()
end