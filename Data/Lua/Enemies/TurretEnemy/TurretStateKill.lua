function KillFirstTurret(args)
	utils_log("KillFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	if l_Enemy.m_ActualAnimation == 1 then
		l_Owner:remove_action(l_Enemy.m_ActualAnimation)
	else
		l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,1.0)
	end
	l_Enemy.m_ActualAnimation = 4
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	local l_Dir = (g_Player.m_RenderableObject:get_position() - l_Owner:get_position())
	l_Dir.y = 0
	l_Dir:normalize(1)
	local auxQuatf = Quatf()
	auxQuatf:set_from_fwd_up(l_Dir,Vect3f(0,1,0))
	l_Owner:set_rotation(auxQuatf)
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	local l_FPSCamera = l_CameraManager:get_main_camera()
	local l_AnimatedCamera = l_CameraManager:get_resource("JaheemDies")
	local l_CameraKey = l_AnimatedCamera:get_camera_key(0)
	local l_CameraInfo = l_CameraKey:get_camera_info()
	local l_CameraInfoPos = l_CameraInfo:get_eye()
	local l_CameraInfoLookAt = l_CameraInfo:get_look_at()
	l_AnimatedCamera.m_PositionOffsetKey = l_CameraInfoPos
	l_AnimatedCamera.m_PositionOffset = l_FPSCamera:get_position()
	local l_Forward = l_FPSCamera:get_rotation():get_forward_vector()
	l_Forward.y = 0
	l_Forward.x = l_Forward.x * -1.0
	l_Forward:normalize(1)
	local aux = Quatf()
	aux:set_from_fwd_up(l_Forward, Vect3f(0,1,0))
	l_AnimatedCamera.m_RotationOffset = aux:rotation_matrix()
	l_AnimatedCamera:reset_time()
	l_CameraManager:choose_main_camera("JaheemDies")
	
	l_Enemy.m_TimerToStop = 0
	
	--local UABEngine = CUABEngine.get_instance()
	--UABEngine:get_layer_manager():get_layer("particles"):get_resource("EmisorParticulaTorreta"):set_visible(true)
end

function KillUpdateTurret(args, _ElapsedTime)
	local l_Enemy = args["self"]	
	local l_Owner = args["owner"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	

	--l_Enemy:RotateEnemyBone(l_Enemy.m_BackBoneId, l_PlayerPos, 1.0)
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	if l_Enemy.m_TimerToStop >= 6.0 then
		l_Enemy.m_State = "off"
	end
end

function KillEndTurret(args)
	utils_log("KillEnd")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraManager:choose_main_camera("MainCamera")
	l_CameraManager:get_main_camera():unlock()
	l_Owner:set_rotation(l_Enemy.m_DefaultRotation)
end

function KillToIdleConditionTurret(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function KillToOffConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "off"
end