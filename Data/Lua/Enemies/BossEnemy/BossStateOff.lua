function OffFirstBoss(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
end

function OffUpdateBoss(args, _ElapsedTime)
	local l_Enemy = args["self"]

	--if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"
	--end
end

function OffEndBoss(args)
	utils_log("OffEnd")
end

function OffToIdleConditionBoss(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end