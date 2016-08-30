function AttackFirstTurret(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	--l_Enemy.m_ActualAnimation = 2
	--l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	
	l_Enemy.m_Timer = 0
end

function AttackUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Enemy.m_RenderableObject:get_position()
	local l_Distance = l_EnemyPos:distance(l_PlayerPos)
	
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
	
	if l_Enemy:PlayerVisible(l_Owner) and l_Distance <= l_Enemy.m_DistanceToAttack then		
		if l_Distance <= l_Enemy.m_DistanceToKill then
			utils_log("ESTAS MUERTO!!!!")
		else
			local l_Direction = (l_PlayerPos - l_EnemyPos):get_normalized(1)	
			local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Owner:get_rotation():get_forward_vector():get_normalized(1), l_Direction)
			
			if angle_to_turn ~= nil then
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
				l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
					
				if l_PercentRotation > 1.0 then
					l_PercentRotation = 1.0
					l_Enemy.m_TimerRotation = 0
				end
				l_Enemy:EnemyRotation(angle_to_turn, l_PercentRotation)
			end
		end
	else
		local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Owner:get_rotation():get_forward_vector():get_normalized(1), l_Enemy.m_DefaultForward)
		if angle_to_turn ~= nil and (angle_to_turn <= 0.10 or angle_to_turn >= 0.15) then
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
			l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
				
			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
				l_Enemy.m_TimerRotation = 0
			end
			l_Enemy:EnemyRotation(angle_to_turn, l_PercentRotation)
		else
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