function AttackFirstCagedAutomaton(args)
	utils_log("AttackFirstCagedAutomaton")
	local l_Owner = args["owner"]
	l_Owner:remove_action(0)
	l_Owner:blend_cycle(2,1.0,0.5)
end

function AttackUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Owner = args["owner"]	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	l_Enemy:RotateEnemyHead(l_Owner, l_PlayerPos)
	
	-- Si la distancia entre el enemy y el player es menor a lo establecido pasamos a attack
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
end

function AttackEndCagedAutomaton(args)
	utils_log("AttackEndCagedAutomaton")
end