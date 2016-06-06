function IdleFirstAutomaton(args)
end

function IdleUpdateAutomaton(args, _ElapsedTime)

end

function IdleEndAutomaton(args)
end

function IdleToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_Patrol
end

function IdleToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	return l_Distance < l_Enemy.m_distance_to_activate
end