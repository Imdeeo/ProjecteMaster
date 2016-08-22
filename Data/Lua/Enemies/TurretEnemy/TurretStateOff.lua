function OffFirstTurret(args)
	utils_log("OffFirst")
end

function OffUpdateTurret(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"	
	end
end

function OffEndTurret(args)
end

function OffToIdleConditionTurret(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end