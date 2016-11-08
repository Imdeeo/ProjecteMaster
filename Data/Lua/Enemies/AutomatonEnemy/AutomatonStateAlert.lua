function AlertFirstAutomaton(args)
	--utils_log("AlertFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 3
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_LastPositionPlayer = nil
end

function AlertUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	if l_Enemy:PlayerVisible(l_Owner) or l_Enemy:DetectPlayerNoise(1.5) then
		-- the enemy has heard to player. How the enemy is in alert he hears more far away
		if l_Enemy:CheckPlayerDistance(l_Enemy.m_DistanceToKill) then
			l_Enemy.m_State = "attack"
		elseif l_Enemy.m_Patrol and not l_Enemy.m_IsChasing then
			l_Enemy:IncrementePatrolPointIndex()
			local l_NodePoint = l_Enemy:GetActualPatrolPoint()
			l_Enemy.m_DefaultPosition = l_NodePoint.node.position
		end
		l_Enemy.m_alert_timer = 0.0
		l_Enemy.m_State = "chase"
	else
		-- if the enemy doesn't see to player, he is in alert until the alert time is closed
		local l_Timer = 0
		
		if l_Enemy.m_Patrol and not l_Enemy.m_IsChasing then
			local l_NodePoint = l_Enemy:GetActualPatrolPoint()
			l_Timer = l_NodePoint.time_to_wait
		else
			l_Timer = l_Enemy.m_StandardAlertTime
		end
		
		l_Enemy.m_alert_timer = l_Enemy.m_alert_timer + _ElapsedTime
	
		if l_Enemy.m_alert_timer > l_Timer then
			l_Enemy.m_alert_timer = 0.0
			if l_Enemy.m_Patrol and not l_Enemy.m_IsChasing then
				l_Enemy:IncrementePatrolPointIndex()
				l_Enemy.m_State = "patrol"
			else
				l_Enemy.m_State = "return"
			end
		end
	end
end

function AlertEndAutomaton(args)
	--utils_log("AlertEnd")
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