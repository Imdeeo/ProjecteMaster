function FogOffFirst(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
    l_Owner:clear_cycle(2,0.1)	
	g_Engine:get_physX_manager():character_controller_teleport(l_Enemy.m_Name, Vect3f(13.5,0.1, 13.9))
	l_Owner:set_visible(false)
	utils_log("FogOffUpdateFirst")
end

function FogOffUpdate(args, _ElapsedTime)
end

function FogOffEnd(args)
	local l_Owner = args["owner"]
	l_Owner:set_visible(true)
end

function FogOffToChaseCondition(args)
	local l_Enemy = args["self"]
	return not l_Enemy.m_Off
end
