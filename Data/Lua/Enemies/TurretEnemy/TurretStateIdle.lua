function IdleFirstTurret(args)
	utils_log("TurretIdleFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	utils_log("IDLE ANIMACION ACTUAL: "..l_Enemy.m_ActualAnimation)
	
	if l_Enemy.m_ActualAnimation == 0 then
		l_Enemy.m_ActualAnimation = 0
	else
		l_Enemy.m_ActualAnimation = 3
		if l_Enemy.m_ActualAnimation == 1 then
			l_Owner:remove_action(l_Enemy.m_ActualAnimation)
		elseif l_Enemy.m_ActualAnimation == 2 then
			l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,1.0)
		end
	end
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	l_Enemy.m_TimerToStop = 0.0
	l_Enemy.m_TimerRotation = 0.0
end

function IdleUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_Direction = (l_Enemy.m_DefaultPosition - l_PlayerPos):get_normalized(1)
	
	if ((l_Enemy.m_ActualAnimation == 0 and l_Enemy.m_TimerToStop >= 4.43) 
		or (l_Enemy.m_ActualAnimation == 3 and l_Enemy.m_TimerToStop >= 3.33))
		and l_Enemy:PlayerVisible(l_Owner) then
		-- check if player is looking to enemy		
		local l_Distance = l_Enemy.m_DefaultPosition:distance(l_PlayerPos)		
		
		local l_PlayerForward = g_Player.m_CameraController:get_forward():get_normalized(1)
		local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Direction, l_PlayerForward)		
		
		if angle_to_turn ~= nil and angle_to_turn >= -0.3 and angle_to_turn <= 0.3 then
			-- playes has to lose sanity because he is looking the enemy
			l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
			l_Enemy:LoseSanity(l_Distance)
		end		
		
		if l_Distance <= l_Enemy.m_DistanceToAttack then
			l_Enemy.m_State = "attack"
		else
			-- the enemy rotates the head bone to player position
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
				
			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime
			end
			
			l_Enemy:RotateEnemyBone(l_Enemy.m_HeadBoneId, l_PlayerPos, l_PercentRotation)
		end
	else
		-- the enemy rotates the head bone to original head position
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime	
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
			
		if l_PercentRotation < 0.0 then
			l_PercentRotation = 0.0
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
		end
		
		l_Enemy:RotateEnemyBone(l_Enemy.m_HeadBoneId, l_PlayerPos, l_PercentRotation)
	end
end

function IdleEndTurret(args)
end

function IdleToAttackConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end