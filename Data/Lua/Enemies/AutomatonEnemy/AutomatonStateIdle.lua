function IdleFirstAutomaton(args)
	utils_log("IdleFirst")
	local l_Owner = args["owner"]	
	local l_Enemy = args["self"]

	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 0
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
	
	l_Enemy.m_Velocity = Vect3f(0,0,0)
end

function IdleUpdateAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())	
	
	l_Enemy:EnemyMove(_ElapsedTime)	
	
	if l_Enemy.m_Patrol then
	
		l_Enemy.m_State = "patrol"
		
	else
		local l_Run = g_Player.m_InputManager:is_action_active("Run")
		local l_Crouch = g_Player.m_InputManager:is_action_active("Crouch")
		
		if l_Enemy.m_Awake == true
			or (l_Run == true and l_Distance <= l_Enemy.m_DistanceToActivateRun)
			or (l_Crouch == false and l_Run == false and l_Distance <= l_Enemy.m_DistanceToActivateWalk)
			or (l_Crouch == true and l_Distance <= l_Enemy.m_DistanceToActivateCrouching) then			
			l_Enemy.m_State = "chase"
		end		
	end
end

function IdleEndAutomaton(args)
end

function IdleToPatrolConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "patrol"
end

function IdleToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end