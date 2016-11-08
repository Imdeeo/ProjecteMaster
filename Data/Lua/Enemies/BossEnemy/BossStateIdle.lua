function IdleFirstBoss(args)
	--utils_log("IdleFirst")
	local l_Owner = args["owner"]	
	local l_Enemy = args["self"]

	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	l_Enemy.m_ActualAnimation = 0
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
end

function IdleUpdateBoss(args, _ElapsedTime)
	local l_Enemy = args["self"]
	
	if l_Enemy.m_Awake then
		l_Enemy.m_State = "patrol"
	elseif l_Enemy.m_Looking then
		l_Enemy.m_State = "looking"
	end
end

function IdleEndBoss(args)
	--utils_log("IdleEnd")
end

function IdleToPatrolConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "patrol"
end

function IdleToLookingConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end