function IdleFirstCagedAutomaton(args)
	utils_log("IdleFirstCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy.m_Break then
		l_Enemy.m_ActualAnimation = 1
	else
		l_Enemy.m_ActualAnimation = 0
	end

	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
end

function IdleUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	if not l_Enemy.m_Break and l_Enemy.m_TimerToStop >= 6.3 then
		l_Enemy.m_State = "attack"
	end
end

function IdleEndCagedAutomaton(args)
	utils_log("IdleEndCagedAutomaton")
end

function IdleToAttackConditionCagedAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end