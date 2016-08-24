function IdleFirstAutomaton(args)
	utils_log("IdleFirst")
	local l_Owner = args["owner"]	
	local l_Enemy = args["self"]

	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 0
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_LastPositionPlayer = nil
	l_Enemy.m_IsChasing = false
end

function IdleUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Enemy.m_Patrol then	
		l_Enemy.m_State = "patrol"		
	else
		if l_Enemy:PlayerVisible(l_Owner) then
			-- the enemy has seen to player
			l_Enemy.m_IsChasing = true
			l_Enemy.m_State = "chase"
		elseif l_Enemy:DetectPlayerNoise(1) then
			l_Enemy.m_State = "chase"
		end
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