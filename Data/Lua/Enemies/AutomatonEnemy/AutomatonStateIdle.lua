function IdleFirstAutomaton(args)
	utils_log("IdleFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(1,0.5)
	l_Owner:clear_cycle(2,0.5)
	l_Owner:clear_cycle(3,0.5)
	l_Owner:blend_cycle(0,1.0,0.5)
	
	local l_Enemy = args["self"]
	l_Enemy.m_Velocity = Vect3f(0,0,0)
end

function IdleUpdateAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())	
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Enemy.m_Patrol then
		l_Enemy.m_State = "patrol"
	elseif l_Distance < l_Enemy.m_distance_to_activate then
		l_Enemy.m_State = "chase"
	end
end

function IdleEndAutomaton(args)
end

function IdleToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "patrol"
end

function IdleToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end