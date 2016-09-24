function OffFirstAutomaton(args)
	utils_log("OffFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy.m_ActualAnimation ~= 4 then
		l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,1.0)
	else
		l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	end
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