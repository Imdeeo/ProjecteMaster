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
	g_Player:SetAnimationCamera("JaheemDies")
	
	l_Enemy.m_TimerToStop = 0

	local UABEngine = CUABEngine.get_instance()
	local l_TurretParticle = UABEngine:get_layer_manager():get_layer("particles"):get_resource("EmisorParticulaTorreta")
	l_TurretParticle:set_start(true)
	l_TurretParticle:set_visible(true)
	local l_EnemyPos = l_Owner:get_position()
	local l_HeadPos = l_Owner:get_bone_position(l_Enemy.m_HeadBoneId) + Vect3f(0,-0.05,0.1)
	l_HeadPos = l_Owner:get_rotation():rotation_matrix() * l_HeadPos
	l_HeadPos.x = l_HeadPos.x * -1
	local l_Aux = (l_EnemyPos + l_HeadPos) / 2
	l_TurretParticle:set_position(l_Aux)
end

function KillUpdateTurret(args, _ElapsedTime)
	local l_Enemy = args["self"]	
	local l_Owner = args["owner"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	if l_Enemy.m_TimerToStop >= 6.0 then
		l_Enemy.m_State = "off"
	end
	
	local UABEngine = CUABEngine.get_instance()
	local l_TurretParticle = UABEngine:get_layer_manager():get_layer("particles"):get_resource("EmisorParticulaTorreta")
	local l_EnemyPos = l_Owner:get_position()
	local l_HeadPos = l_Owner:get_bone_position(l_Enemy.m_HeadBoneId) + Vect3f(0,-0.05,0.1)
	l_HeadPos = l_Owner:get_rotation():rotation_matrix() * l_HeadPos
	l_HeadPos.x = l_HeadPos.x * -1
	local l_Aux = (l_EnemyPos + l_HeadPos) / 2
	l_TurretParticle:set_position(l_Aux)
end

function KillEndTurret(args)
	utils_log("KillEnd")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraManager:choose_main_camera("MainCamera")
	l_CameraManager:get_main_camera():unlock()
	l_Owner:set_rotation(l_Enemy.m_DefaultRotation)
	local UABEngine = CUABEngine.get_instance()
	local l_TurretParticle = UABEngine:get_layer_manager():get_layer("particles"):get_resource("EmisorParticulaTorreta")
	l_TurretParticle:set_visible(false)
	l_TurretParticle:set_start(false)
end

function KillToIdleConditionTurret(args)	
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function KillToOffConditionTurret(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "off"
end