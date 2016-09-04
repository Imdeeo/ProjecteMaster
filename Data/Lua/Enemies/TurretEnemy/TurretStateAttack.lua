function AttackFirstTurret(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	utils_log("ATTACK ANIMACION ACTUAL: "..l_Enemy.m_ActualAnimation)
	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	l_Enemy.m_Timer = 0
	l_Enemy.m_TimerToStop = 0
	l_Enemy.m_TimerRotation = 0
end

function AttackUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Enemy.m_RenderableObject:get_position()
	local l_Distance = l_EnemyPos:distance(l_PlayerPos)	
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	if l_Enemy.m_TimerToStop >= 1.33 and l_Enemy.m_ActualAnimation == 1 then
		l_Owner:remove_action(l_Enemy.m_ActualAnimation)
		l_Enemy.m_ActualAnimation = 2
		l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	end
	
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
	
	if l_Enemy:PlayerVisible(l_Owner) and l_Distance <= l_Enemy.m_DistanceToAttack then	
		if l_Distance <= l_Enemy.m_DistanceToKill then
			utils_log("ESTAS MUERTO!!!!")
		else
			if l_Enemy.m_ActualAnimation == 2 then
				-- the enemy rotates the body to player position
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
				local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
					
				if l_PercentRotation > 1.0 then
					l_PercentRotation = 1.0
					l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime
				end
				
				l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, l_PercentRotation)
			end
		end
	else
		-- the enemy rotates the body to original position
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime	
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
			
		if l_PercentRotation < 0.0 then
			l_PercentRotation = 0.0
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
		end
		
		l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, l_PercentRotation)
		
		if l_PercentRotation == 0.0 then
			l_Enemy.m_State = "idle"
		end
	end
end

function AttackEndTurret(args)
	utils_log("AttackEnd")
end

function AttackToIdleConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end