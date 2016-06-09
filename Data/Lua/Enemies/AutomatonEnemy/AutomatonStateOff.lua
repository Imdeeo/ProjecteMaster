function OffFirstAutomaton(args)
	utils_log("OffFirst")
end

function OffUpdateAutomaton(args, _ElapsedTime)
	
end

function OffEndAutomaton(args)
end

function OffToIdleConditionAutomaton(args)	
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	return l_Distance < l_Enemy.m_distance_to_activate or l_Enemy.m_Patrol
end