function OffFirstTurret(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy.m_ActualAnimation ~= 2 then
		l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	else
		l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,1.0)
	end
	l_Enemy.m_ActualAnimation = 0
end

function OffUpdateTurret(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"	
	end
end

function OffEndTurret(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	l_Enemy.m_SoundManager:play_event(g_AutomatonActivateSoundEvent, l_Owner)
end

function OffToIdleConditionTurret(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end
