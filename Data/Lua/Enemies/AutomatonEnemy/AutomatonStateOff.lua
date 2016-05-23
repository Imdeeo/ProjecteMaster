function OffFirstAutomaton(args)
	utils_log("OffFirst")	
end

function OffUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Owner:get_position())
	
	if l_Distance < 10.0 then
		utils_log("se mueve")
		l_Owner:blend_cycle(0,1.0,0.1)
	else
		utils_log("muevete cerdo")
	end
end

function OffEndAutomaton(args)
	utils_log("OffEnd")
end

function OffToIdleConditionAutomaton()
	--return (g_Player.m_InputManager:is_action_active("MOVE_FWD") or g_Player.m_InputManager:is_action_active("MOVE_BACK") or g_Player.m_InputManager:is_action_active("STRAFE_LEFT") or g_Player.m_InputManager:is_action_active("STRAFE_RIGHT"))
end