function KillFirstBoss(args)
	utils_log("KillFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
		
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,1.0)
	l_Enemy.m_ActualAnimation = 4
	l_Owner:execute_action(l_Enemy.m_ActualAnimation,0.5,0.5,1.0,true)
	
	local l_Dir = (g_Player.m_RenderableObject:get_position() - l_Owner:get_position())
	l_Dir.y = 0
	l_Dir:normalize(1)
	local auxQuatf = Quatf()
	auxQuatf:set_from_fwd_up(l_Dir,Vect3f(0,1,0))
	l_Owner:set_rotation(auxQuatf)
	g_Player:SetAnimationCamera("JaheemDiesBoss")
	
	l_Enemy.m_Timer = 0
	l_Enemy.m_TimerToStop = 0
	
	l_Enemy.m_LoseSanityValue = g_Player.m_Sanity / 10.0
	
	g_SoundManager:play_event(g_ButhcerSoundEvent)
end

function KillUpdateBoss(args, _ElapsedTime)
	local l_Enemy = args["self"]
	local l_Owner = args["owner"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	l_Enemy.m_TimerToStop = l_Enemy.m_TimerToStop + _ElapsedTime
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	
	if l_Enemy.m_Timer >= 1.0 then
		g_Player:ModifySanity(-l_Enemy.m_LoseSanityValue)
		l_Enemy.m_Timer = l_Enemy.m_Timer - 1.0
	end
	
	if l_Enemy.m_TimerToStop >= 10.5 then
		l_Enemy.m_State = "off"
	end
end

function KillEndBoss(args)
	utils_log("KillEnd")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraManager:choose_main_camera("MainCamera")
	l_CameraManager:get_main_camera():unlock()
	l_Owner:set_rotation(l_Enemy.m_DefaultRotation)
end

function KillToOffConditionBoss(args)
	local l_Enemy = args["self"]
	l_Enemy.m_Awake = false
	return l_Enemy.m_State == "off"
end