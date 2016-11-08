function OffFirstCagedAutomaton(args)
	--utils_log("OffFirstCagedAutomaton")
end

function OffUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())

	if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"
	end
end

function OffEndCagedAutomaton(args)
	--utils_log("OffEndCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	l_Enemy.m_SoundManager:play_event(g_AutomatonActivateSoundEvent, l_Owner)
end

function OffToIdleConditionCagedAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end
