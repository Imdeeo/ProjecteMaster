function FogOffFirst(args)
	local l_Owner = args["owner"]
    l_Owner:clear_cycle(0,0.1)	
end

function FogOffUpdate(args, _ElapsedTime)
end

function FogOffEnd(args)
end

function FogOffToTeleportCondition(args)
	local l_Enemy = args["self"]
	return m_CharacterManager.m_Enemics[1].m_off == false
end
