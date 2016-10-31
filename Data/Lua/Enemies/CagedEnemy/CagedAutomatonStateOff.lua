function OffFirstCagedAutomaton(args)
	utils_log("OffFirstCagedAutomaton")
end

function OffUpdateCagedAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())

	if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"
	end
end

function OffEndCagedAutomaton(args)
	utils_log("OffEndCagedAutomaton")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_DelayRTPC = SoundRTPC()
	l_DelayRTPC.rtpc_name = "initial_delay"
	l_Enemy.m_SoundManager:set_rtpc_value(l_DelayRTPC, 3.5 + math.random(), l_Owner)
	l_Enemy.m_SoundManager:play_event(l_Enemy.m_ActivateSoundEvent, l_Owner)
end

function OffToIdleConditionCagedAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end
