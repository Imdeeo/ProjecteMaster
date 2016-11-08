function PatrolFirstBoss(args)
	--utils_log("PatrolFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 2
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	local l_SoundSync = CSoundSynchronizer(l_Enemy, l_Owner, g_BossStepSoundEvent) 
	l_SoundSync.m_RepeatsPerCycle = 2 
	l_SoundSync.m_Offset = 0.20 
	l_Enemy.m_SoundSync = l_SoundSync 
	
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_Timer = 0.0
	l_Enemy.m_TimerRotation = 0.0
end

function PatrolUpdateBoss(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy.m_Timer >= 6.6 then
		if l_Enemy.m_ActualAnimation == 3 then
			if l_Enemy:CheckPlayerDistance(l_Enemy.m_DistanceToKill) then
				l_Enemy.m_State = "attack"
			else
				local l_NodePoint = l_Enemy:GetActualPatrolPoint()
				local l_PointPos = l_NodePoint.node.position
				local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)
				
				----utils_log("Distance Boss: "..l_Distance)
				if l_Distance <= l_Enemy.m_DistanceToChangeNodeWalking then
					if l_Enemy.m_IndexPathPatrolPoint < l_Enemy.m_TotalPatrolNodes - 1 then
						l_Enemy:IncrementePatrolPointIndex()
						l_Enemy.m_TimerRotation = 0.0
					else
						l_Enemy.m_State = "Off"
						l_Enemy.m_Awake = false
					end
				else
					l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
					local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularWalkSpeed
					
					if l_PercentRotation > 1.0 then
						l_PercentRotation = 1.0
					end 
					
					l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
				end
			end
		else
			l_Owner:remove_action(l_Enemy.m_ActualAnimation)
			l_Enemy.m_ActualAnimation = 3
			l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		end
	else
		l_Enemy:EnemyMove(_ElapsedTime)
		l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	end
end

function PatrolEndBoss(args)
end

function PatrolToAttackConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end

function PatrolToOffConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "Off"
end