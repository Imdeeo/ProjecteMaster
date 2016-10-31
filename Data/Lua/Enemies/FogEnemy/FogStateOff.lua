function FogOffFirst(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
    l_Owner:clear_cycle(2,0.1)	
	g_Engine:get_physX_manager():character_controller_teleport(l_Enemy.m_Name, l_Enemy.m_OffPos)
	l_Owner:set_visible(false)
	l_Enemy.m_State="Off" 
	l_Enemy.m_Timer = 0
end

function FogOffUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]


	if l_Enemy.m_Awake then
		l_Enemy.m_Timer = l_Enemy.m_Timer+_ElapsedTime
		if l_Enemy.m_Timer >= l_Enemy.m_TimeToTeleport then
			local didHit = g_Player.m_PhysXManager:raycast(g_Player.m_CameraController:get_position(), g_Player.m_CameraController:get_position()+(g_Player.m_CameraController:get_forward()*(l_Enemy.m_TeleportDistance+1.0)), l_Enemy.m_PhysXGroups, g_Player.m_RaycastData)
			if not didHit then 
				l_Enemy.m_State="Chase"
			end
		end
	else
		l_Enemy.m_Timer = 0
	end
	
end

function FogOffEnd(args)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Enemy.m_ActualAnimation = 2
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_IsCorrected = false
	
	local l_FPSCamera = g_Engine:get_camera_controller_manager():get_main_camera()
	local l_Forward = l_FPSCamera:get_rotation():get_forward_vector()
	l_Forward.y = 0
	l_Forward.x = l_Forward.x * -1.0
	l_Forward:normalize(1)
		
	local aux = Quatf()
	aux:set_from_fwd_up(l_Forward, Vect3f(0,1,0))
	
	local l_Pos = l_Enemy.m_PhysXManager:get_character_controler_pos("player") + aux:rotation_matrix()*Vect3f(0.0,0.0,l_Enemy.m_TeleportDistance)
	l_Pos.y = l_Pos.y - g_StandingOffset+0.1
	g_Engine:get_physX_manager():character_controller_teleport(l_Enemy.m_Name, l_Pos)
	l_Pos.y = l_Pos.y - g_StandingOffset+0.1
	l_Owner:set_position(l_Pos)

	local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
	local l_EnemyPos = l_Owner:get_position()
	local l_Direction = (g_Player.m_RenderableObject:get_position() - l_EnemyPos):get_normalized(1)
	local angle_to_turn = l_Enemy:CalculateAngleRotation(l_EnemyForward, l_Direction)
	local quat_to_turn = Quatf()
	quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)
	local target_quat = l_Owner:get_rotation() * quat_to_turn
	l_Owner:set_rotation(target_quat)
	
	l_Owner:set_visible(true)
end

function FogOffToChaseCondition(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State=="Chase" 
end

function FogAnyToIdle(args)
	local l_Enemy = args["self"]
	return not l_Enemy.m_Awake
end

