function AttackFirstCagedAutomaton(args)
	utils_log("AttackFirstCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]

	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	l_Enemy.m_ActualAnimation = 2
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,0.5,0.5)

	local l_SoundSync = CSoundSynchronizer(l_Enemy, l_Owner, 2)
	l_SoundSync.m_EventsPerCycle = 3
	local l_SE = SoundEvent()
	l_SE.event_name = "Automaton_Beat_6"
	l_SoundSync.m_StartSequenceEvent = l_SE
	l_SoundSync.m_Offset = 0.60
	l_SoundSync.m_AnimationDuration = 2.4
	l_Enemy.m_SoundSync = l_SoundSync
end

function AttackUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Owner = args["owner"]	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()	
	
	l_Enemy.m_SoundSync:Sync(_ElapsedTime)
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
	utils_log("AttackEndCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	l_Enemy.m_SoundManager:play_event(l_Enemy.m_StopBeatSoundEvent, l_Owner)
end
