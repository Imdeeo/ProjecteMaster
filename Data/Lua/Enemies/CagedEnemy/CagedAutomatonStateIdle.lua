function IdleFirstCagedAutomaton(args)
	utils_log("IdleFirstCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(1,0.5)
	if l_Enemy.m_Break then
		l_Owner:execute_action(1,0.5,0.5,1.0,true)
	else
		l_Owner:execute_action(0,0.5,0.5,1.0,true)
	end
end

function IdleUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	l_Enemy.m_timer_to_stop = l_Enemy.m_timer_to_stop + _ElapsedTime
	if l_Enemy.m_timer_to_stop >= 6.5 then
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