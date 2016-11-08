function LookingFirstBoss(args)
	--utils_log("LookingFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	l_Enemy.m_Timer = 0.0
end

function LookingUpdateBoss(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	
	if l_Enemy.m_Timer >= 6.6 then
		l_Enemy.m_State = "idle"
	end
end

function LookingEndBoss(args)
	--utils_log("LookingEnd")
end

function LookingToIdleConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end