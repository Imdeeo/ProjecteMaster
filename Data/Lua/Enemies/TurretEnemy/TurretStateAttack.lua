function AttackFirstTurret(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	l_Enemy.m_Timer = 0
	l_Enemy.m_TimerToStop = 0
	l_Enemy.m_TimerRotation = 0
	l_Enemy.m_IsKilling = false
end

function AttackUpdateTurret(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Enemy.m_RenderableObject:get_position()
	
	if not l_Enemy.m_IsKilling then
		local l_Distance = l_EnemyPos:distance(l_PlayerPos)	
		
		l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
		if l_Enemy.m_TimerToStop >= 1.33 and l_Enemy.m_ActualAnimation == 1 then
			l_Owner:remove_action(l_Enemy.m_ActualAnimation)
			l_Enemy.m_ActualAnimation = 2
			l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		end
		
		l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
		l_Enemy:LoseSanity(l_Distance)	
	
		if l_Enemy:PlayerVisible(l_Owner) and l_Distance <= l_Enemy.m_DistanceToAttack then	
			if l_Distance <= l_Enemy.m_DistanceToKill and l_Enemy.m_ActualAnimation == 2 then
				utils_log("ESTAS MUERTO!!!!")				
				l_Enemy.m_IsKilling = true
				
				local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
				l_AnimatedCamera = l_CameraManager:get_resource("JaheemDies")
				local l_CameraInfoPosLength = l_AnimatedCamera:get_camera_key(0):get_camera_info():get_eye()
				l_CameraInfoPosLength.y = 0
				l_CameraInfoPosLength = l_CameraInfoPosLength:length()
	
				g_Player.m_CameraController:lock()
				local l_auxTarget = l_PlayerPos - l_EnemyPos
				l_auxTarget.y = 0
				g_Player.m_Target = l_auxTarget:get_normalized(1) * l_CameraInfoPosLength + l_EnemyPos
				
				l_Enemy.m_TimerRotation = 0
				l_Enemy.m_InitialCameraRotation = g_Player.m_CameraController:get_rotation()
				
				local l_CameraDirection = (l_EnemyPos - g_Player.m_PhysXManager:get_character_controler_pos("player"))
				l_CameraDirection.y = 0
				l_CameraDirection = l_CameraDirection:get_normalized(1)
								
				local quat_to_turn = Quatf()
				quat_to_turn:set_from_fwd_up(l_CameraDirection, Vect3f(0,1,0))
				l_Enemy.m_FinalCameraRotation = quat_to_turn
			else
				if l_Enemy.m_ActualAnimation == 2 then
					-- the enemy rotates the body to player position
					l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
					local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
						
					if l_PercentRotation > 1.0 then
						l_PercentRotation = 1.0
						l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime
					end
					
					l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, l_PercentRotation)
				end
			end
		else
			-- the enemy rotates the body to original position
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation - _ElapsedTime	
			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularSpeed
					
			if l_PercentRotation < 0.0 then
				l_PercentRotation = 0.0
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
			end
			
			l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, l_PercentRotation)
			
			if l_PercentRotation == 0.0 then
				l_Enemy.m_State = "idle"
			end
		end
	else	
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
		local l_PercentRotation = l_Enemy.m_TimerRotation / 2.0
		if l_PercentRotation > 1.0 then
			l_PercentRotation = 1.0
		end
		
		l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, 1.0)
		
		local l_AngleOK = false
		local l_PosOK = false
	
		local l_FaceTargetDisplacement =  g_Player.m_Target - g_Player.m_PhysXManager:get_character_controler_pos("player")
		l_FaceTargetDisplacement.y = 0.0
		
		--utils_log("LENGTH: "..l_FaceTargetDisplacement:length())
		if l_FaceTargetDisplacement:length() <= 0.01 then
			l_PosOK = true
		else
			g_Player.m_PhysXManager:character_controller_move("player", l_FaceTargetDisplacement:get_normalized(1), _ElapsedTime)
		end		
		
		if l_Enemy.m_TimerRotation <= 2.0 then
			local target_quat = l_Enemy.m_InitialCameraRotation:slerp(l_Enemy.m_FinalCameraRotation, l_PercentRotation)
			g_Player.m_CameraController:set_rotation(target_quat)
		else
			l_AngleOK = true
		end		

		if l_PosOK and l_AngleOK then
			--g_Player.m_CameraController:unlock()		
			--g_Player.m_CameraController:set_up(Vect3f(0,1,0))
			l_Enemy.m_State = "kill"
		end
		--[[local auxPos = l_Owner:get_position()+Vect3f(0,0,-1.34996)
		auxPos.y = g_Player.m_RenderableObject:get_position().y
		g_Player.m_RenderableObject:set_position(auxPos)
		g_Player.m_CameraController:set_rotation(Quatf(0.0,0.0,0.0,1))
		l_Enemy.m_State = "kill"]]
	end
end

function AttackEndTurret(args)
	utils_log("AttackEnd")
	local l_Enemy = args["self"]
	if l_Enemy.m_State == "kill" then 
		g_Player.m_CameraController:unlock()
	end
end

function AttackToIdleConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function AttackToKillConditionTurret(args)
	local l_Enemy = args["self"]	
	return l_Enemy.m_State == "kill"
end