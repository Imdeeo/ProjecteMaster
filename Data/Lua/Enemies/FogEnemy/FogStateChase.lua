function FogChaseFirst(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 2
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	utils_log("FogChaseFirst")
end

function FogChaseUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
	
	l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
	
	local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
		
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
		l_Enemy.m_TimerRotation = 0.0
	end 
		
	l_Enemy:EnemyWalk(l_PlayerPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)		
	
	-- Si la distancia entre el enemy y el player es menor a lo establecido pasamos a attack
	if l_Distance < l_Enemy.m_DistanceToKill then
		l_Enemy.m_Attack = true
	end
end

function FogChaseEnd(args)
end

function FogChaseToOffCondition(args)
	local l_Enemy = args["self"]
	local l_EnemyPos = l_Enemy.m_RenderableObject:get_position()
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_PlayerForward = g_Player.m_RenderableObject:get_rotation():get_forward_vector():get_normalized(1)
	local l_Direction = (l_EnemyPos-l_PlayerPos):get_normalized(1)
	local l_Angle = l_PlayerForward * l_Direction
	if l_Angle < 0.35 then
		l_Enemy.m_Off = true
		return true
	else
		return false
	end
end

function FogChaseToAttackCondition(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_Attack
end