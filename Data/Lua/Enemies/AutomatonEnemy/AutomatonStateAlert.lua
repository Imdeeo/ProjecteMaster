function AlertFirstAutomaton(args)
	utils_log("AlertFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(1,0.5)
	l_Owner:clear_cycle(2,0.5)
	l_Owner:blend_cycle(0,1.0,0.5)
end

function AlertUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]	
	
	if l_Enemy:PlayerVisible(l_Owner) then
		l_Enemy.m_State = "chase"
		l_Enemy.m_alert_timer = 0.0
	else
		local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point("path1")
	
		l_Enemy.m_alert_timer = l_Enemy.m_alert_timer+_ElapsedTime
	
		if l_Enemy.m_alert_timer > l_NodePoint.time_to_wait then
			l_Enemy.m_alert_timer = 0.0
			
			if l_Enemy.m_Patrol then
				l_Enemy.m_PathFindig:increment_actual_patrol_point("path1")
				l_Enemy.m_State = "patrol"
			else
				l_Enemy.m_State = "return"
			end
		end
	end
end

function AlertEndAutomaton(args)
	utils_log("AlertEnd")
end

function AlertToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end

function AlertToReturnConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "return"
end

function AlertToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "patrol"
end