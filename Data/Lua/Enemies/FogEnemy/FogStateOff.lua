function FogOffFirst(args)
	utils_log("FogOffFirst")
end

function FogOffUpdate(args, _ElapsedTime)
	utils_log("FogOffUpdate")
end

function FogOffEnd(args)
	utils_log("FogOffEnd")
end

function FogOffToTeleportCondition()
	return m_CharacterManager.m_Enemics[1].m_off == false
end
