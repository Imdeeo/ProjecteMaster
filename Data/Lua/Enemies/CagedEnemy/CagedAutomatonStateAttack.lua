function AttackFirstCagedAutomaton(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]

	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	l_Enemy.m_ActualAnimation = 2
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,0.5,0.5)
end

function AttackUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Owner = args["owner"]	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()	
	
	-- the enemy rotates the head bone to player position
	l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
	local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
		
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime
	end
	
	l_Enemy:RotateEnemyBone(l_Enemy.m_HeadBoneId, l_PlayerPos, l_PercentRotation)
	
	-- Si la distancia entre el enemy y el player es menor a lo establecido pasamos a attack
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
end

function AttackEndCagedAutomaton(args)
end