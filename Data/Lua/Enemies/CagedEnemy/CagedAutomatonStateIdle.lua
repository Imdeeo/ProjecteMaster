function IdleFirstCagedAutomaton(args)
	utils_log("IdleFirstCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(1,0.5)
	if l_Enemy.m_Break then
		l_Owner:execute_action(1,0,0,1.0,true)
	else
		l_Owner:execute_action(0,0,0,1.0,true)
	end
end

function IdleUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Owner = args["owner"]
	
	l_Enemy.m_timer_to_stop = l_Enemy.m_timer_to_stop + _ElapsedTime
	if l_Enemy.m_timer_to_stop >= 6.7 then
		l_Owner:remove_action(0)
		l_Owner:blend_cycle(2,1.0,1.0)
	end
end

function IdleEndCagedAutomaton(args)
end