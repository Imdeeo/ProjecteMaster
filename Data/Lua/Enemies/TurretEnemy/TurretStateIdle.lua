function IdleFirstTurret(args)
	utils_log("TurretIdleFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(1,0.5)
	l_Owner:clear_cycle(2,0.5)
	l_Owner:blend_cycle(0,1.0,0.5)
end

function IdleUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy:PlayerVisible(l_Owner) then
		local l_PlayerPos = g_Player.m_RenderableObject:get_position()
		local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
		
		if l_Distance <= 2.0 then
			l_Enemy.m_State = "attack"
		else
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularWalkSpeed
			
			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
				l_Enemy.m_TimerRotation = 0.0
			end 
			
			l_Enemy:EnemyWalk(l_Owner, l_Owner:get_position(), l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
		end
	end
end

function IdleEndTurret(args)
end

function IdleToAttackConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end