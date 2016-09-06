function OffFirstAutomaton(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
end

function OffUpdateAutomaton(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	l_Enemy:EnemyMove(_ElapsedTime)

	if l_Enemy.m_Awake then
		l_Enemy.m_State = "idle"
	end
end

function OffEndAutomaton(args)
end

function OffToIdleConditionAutomaton(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end