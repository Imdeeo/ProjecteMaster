function AlertFirstAutomaton(args)
	utils_log("AlertFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(1,0.5)
end

function AlertUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point("path1")
	
	l_Enemy.m_alert_timer = l_Enemy.m_alert_timer+_ElapsedTime	
end

function AlertEndAutomaton(args)
	utils_log("AlertEnd")
end

function AlertToChaseConditionAutomaton(args)
end

function AlertToReturnConditionAutomaton(args)
end

function AlertToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point("path1")
	
	if l_Enemy.m_alert_timer > l_NodePoint.time_to_wait then
		l_Enemy.m_PathFindig:increment_actual_patrol_point("path1")
		return true
	else
		return false
	end
end