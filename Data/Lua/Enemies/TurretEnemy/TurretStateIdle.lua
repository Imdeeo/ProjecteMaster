function IdleFirstTurret(args)
	utils_log("TurretIdleFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]

	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 0
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
end

function IdleUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy:PlayerVisible(l_Owner) then
		
		-- check if player is looking to enemy
		local l_PlayerPos = g_Player.m_RenderableObject:get_position()
		local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
		local l_Direction = (l_Enemy.m_DefaultPosition - l_PlayerPos):get_normalized(1)
		
		local l_PlayerForward = g_Player.m_CameraController:get_forward():get_normalized(1)
		local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Direction, l_PlayerForward)		
		
		if angle_to_turn ~= nil and angle_to_turn >= -0.3 and angle_to_turn <= 0.3 then
			l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
			l_Enemy:LoseSanity(l_Distance)
		end		
		
		if l_Distance <= l_Enemy.m_DistanceToAttack then
			l_Enemy.m_State = "attack"
		else
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
			
			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
				l_Enemy.m_TimerRotation = 0.0
			end 
			
			l_Enemy:RotateEnemyHead(l_Owner, l_PlayerPos)
		end
	end
end

function IdleEndTurret(args)
end

function IdleToAttackConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end