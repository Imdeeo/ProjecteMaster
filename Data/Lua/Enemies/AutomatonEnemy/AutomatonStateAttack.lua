function AttackFirstAutomaton(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_TimerRotation = 0
	g_Player.m_CameraController:lock()
	g_Player:CalculateCameraPositionRotation("JaheemDies2", l_Owner:get_position())
	g_Player.m_IsDead = true
end

function AttackUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
	local l_PercentRotation = l_Enemy.m_TimerRotation / 2.0
	if l_PercentRotation > 1.0 then
		l_PercentRotation = 1.0
	end
	
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	l_Enemy:EnemyWalk(l_PlayerPos, 0, l_PercentRotation, _ElapsedTime)
	
	local l_AngleOK = false
	local l_PosOK = false

	local l_FaceTargetDisplacement =  g_Player.m_Target - g_Player.m_PhysXManager:get_character_controler_pos("player")
	l_FaceTargetDisplacement.y = 0.0
	
	if l_FaceTargetDisplacement:length() <= 0.01 then
		l_PosOK = true
	else
		g_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
	end		
	
	if l_Enemy.m_TimerRotation <= 2.0 then
		local target_quat = g_Player.m_InitialCameraRotation:slerp(g_Player.m_FinalCameraRotation, l_PercentRotation)
		g_Player.m_CameraController:set_rotation(target_quat)
	else
		l_AngleOK = true
	end		

	if l_PosOK and l_AngleOK then
		l_Enemy.m_State = "kill"
	end
end

function AttackEndAutomaton(args)
	utils_log("AttackEnd")
end

function AttackToKillConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "kill"
end