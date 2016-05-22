function FogTeleportFirst(args)
	utils_log("FogTeleportFirst")
end

function FogTeleportUpdate(args, _ElapsedTime)
	utils_log("FogTeleportUpdate")
end

function FogTeleportEnd(args)
	utils_log("FogTeleportEnd")
end

function FogTeleportToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end
