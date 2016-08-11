function OffFirstAutomaton(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
end

function OffUpdateAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Distance < l_Enemy.m_distance_to_activate or l_Enemy.m_Patrol then
		l_Enemy.m_State = "idle"
	end
end

function OffEndAutomaton(args)
end

function OffToIdleConditionAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end