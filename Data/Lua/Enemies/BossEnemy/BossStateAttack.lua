function AttackFirstBoss(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_TimerRotation = 0.0
	g_Player.m_CameraController:lock()
	g_Player:CalculateCameraPositionRotation2("JaheemDiesBoss", l_Owner:get_position())
	g_Player.m_IsDead = true
end

function AttackUpdateBoss(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	g_Engine:get_physX_manager():change_rigid_dynamic_actor_group("player","DeadPlayer")
	
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
	
	utils_log("Distance: "..l_FaceTargetDisplacement:length())
	if l_FaceTargetDisplacement:length() <= 0.01 then
		l_PosOK = true
	else
		g_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
		--// Assign to the character the controller's position
		local l_NewControllerPosition = g_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
		g_Player.m_RenderableObject:set_position(l_NewControllerPosition)
	end		
	
	if l_Enemy.m_TimerRotation <= 2.0 then
		local target_quat = g_Player.m_InitialCameraRotation:slerpJU(g_Player.m_FinalCameraRotation, l_PercentRotation)
		g_Player.m_CameraController:set_rotation(target_quat)
	else
		l_AngleOK = true
	end		

	if l_PosOK and l_AngleOK then
		l_Enemy.m_State = "kill"
	end
end

function AttackEndBoss(args)
	utils_log("AttackEnd")
	local l_Enemy = args["self"]
	if l_Enemy.m_State == "kill" then 
		g_Player.m_CameraController:unlock()
	end
end

function AttackToKillConditionBoss(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "kill"
end