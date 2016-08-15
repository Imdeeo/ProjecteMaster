function AlertFirstAutomaton(args)
	utils_log("AlertFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 3
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
end

function AlertUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Enemy:PlayerVisible(l_Owner) then
		l_Enemy.m_State = "chase"
		l_Enemy.m_alert_timer = 0.0
	else
		local l_Timer = 0
		
		if l_Enemy.m_Patrol and l_Enemy.m_IsChasing == false then			
			local l_NodePoint = l_Enemy.m_PathFindig:get_actual_patrol_point(l_Enemy.m_PatrolName)
			l_Timer = l_NodePoint.time_to_wait
		else
			l_Timer = l_Enemy.m_StandardAlertTime
		end
		
		l_Enemy.m_alert_timer = l_Enemy.m_alert_timer + _ElapsedTime
	
		if l_Enemy.m_alert_timer > l_Timer then
			l_Enemy.m_alert_timer = 0.0
			if l_Enemy.m_Patrol and l_Enemy.m_IsChasing == false then
				l_Enemy.m_PathFindig:increment_actual_patrol_point(l_Enemy.m_PatrolName)
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