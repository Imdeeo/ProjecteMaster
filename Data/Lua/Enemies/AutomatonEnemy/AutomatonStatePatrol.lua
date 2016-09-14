function PatrolFirstAutomaton(args)
	utils_log("PatrolFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_IsChasing = false
end

function PatrolUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy:PlayerVisible(l_Owner) or l_Enemy:DetectPlayerNoise(1) then
		l_Enemy.m_DefaultPosition = Vect3f(l_Owner:get_position().x, l_Owner:get_position().y, l_Owner:get_position().z)
		l_Enemy.m_DefaultRotation = l_Owner:get_rotation()		
		l_Enemy.m_IsChasing = true
		l_Enemy.m_State = "chase"
	else
		local l_NodePoint = l_Enemy:GetActualPatrolPoint()
		local l_PointPos = l_NodePoint.node.position
		local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
		
		if l_Distance <= l_Enemy.m_DistanceToChangeNodeWalking and l_NodePoint.wait == false then
			l_Enemy:IncrementePatrolPointIndex()
			l_Enemy.m_TimerRotation = 0.0
		elseif l_Distance <= 0.3 and l_NodePoint.wait then
			l_Enemy.m_State = "alert"
		else
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularWalkSpeed		
			
			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
			end 
			
			l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)		
		end	
	end
end

function PatrolEndAutomaton(args)
end

function PatrolToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end

function PatrolToAlertConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "alert"
end