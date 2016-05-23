function FogChaseFirst(args)
	utils_log("Chase First")
end

function FogChaseUpdate(args, _ElapsedTime)
	
end

function FogChaseEnd(args)
	utils_log("Chase end")
end

function FogChaseToTeleportCondition()
	return false
end

function FogChaseToOffCondition()
	return m_CharacterManager.m_Enemics[1].m_off == true
end
