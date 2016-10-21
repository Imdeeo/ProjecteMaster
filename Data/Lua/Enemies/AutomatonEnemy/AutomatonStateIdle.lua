function IdleFirstAutomaton(args)
	utils_log("IdleFirst")
	local l_Owner = args["owner"]	
	local l_Enemy = args["self"]

	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 0
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_LastPositionPlayer = nil
end

function IdleUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Enemy.m_Patrol then
		l_Enemy.m_State = "patrol"		
	else
		if (l_Enemy:PlayerVisible(l_Owner) or l_Enemy:DetectPlayerNoise(1)) then
			if l_Enemy:CheckPlayerDistance(l_Enemy.m_DistanceToKill) then
				l_Enemy.m_State = "attack"
			else
				l_Enemy.m_State = "chase"
			end
		end
	end
end

function IdleEndAutomaton(args)
	utils_log("IdleEnd")
end

function IdleToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "patrol"
end

function IdleToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end