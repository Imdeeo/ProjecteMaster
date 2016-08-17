function OffFirstAutomaton(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
end

function OffUpdateAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	l_Enemy:EnemyMove(_ElapsedTime)
	
	local l_Run = g_Player.m_InputManager:is_action_active("Run")
	local l_Crouch = g_Player.m_InputManager:is_action_active("Crouch")
	
	if l_Enemy.m_Awake
		or (l_Run and l_Distance <= l_Enemy.m_DistanceToActivateRun)
		or (not l_Crouch and not l_Run and l_Distance <= l_Enemy.m_DistanceToActivateWalk)
		or (l_Crouch and l_Distance <= l_Enemy.m_DistanceToActivateCrouching) then		
		l_Enemy.m_State = "idle"	
	end
end

function OffEndAutomaton(args)
end

function OffToIdleConditionAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end