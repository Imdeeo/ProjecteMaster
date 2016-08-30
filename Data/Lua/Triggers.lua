function OnChapelEnter(a, b)
	m_CharacterManager.m_Enemics[1].m_off = false
end

function OnChapelExit(a, b)
	m_CharacterManager.m_Enemics[1].m_off = true
end

function OnChapelStay(a, b)

end

function OnExitClimbing(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	l_Player.m_IsInteracting = false
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
end

function OnStayClimbing(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	if (_ColliderName == l_Player.m_Name and l_Player.m_InputManager:is_action_active("INTERACT") and not l_Player.m_IsCorrecting) then
		CheckIfFacing(l_Player, _TriggerName)
	end
end

function CheckIfFacing(_Player, _TriggerName)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource(_TriggerName):get_position()
	--// Check if angle between player camera forward and trigger offset is 180+- 65 degrees (<2.0 rads)
	--// Then checks if angle between the vector triggerPos->playerPos and offset is less than 90 degrees (1.57 rads)
	if (_Player.m_CameraController:get_forward():get_normalized(1):get_angle_with(_Player.m_TargetOffset) > 2.0 and (_Player.m_PhysXManager:get_character_controler_pos("player") - l_Target):get_normalized(1):get_angle_with(_Player.m_TargetOffset) < 1.57) then
		_Player.m_Target = l_Target
		_Player.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
		_Player.m_IsInteracting = false
		_Player.m_IsClimbing = true
		_Player.m_IsCorrecting = true
	end
end

function FogTriggerEnter(_Player, _TriggerName)
	g_Engine:get_physX_manager():character_controller_teleport(m_CharacterManager.m_Fog.m_Name, Vect3f(8.4,0, 3.9))
	m_CharacterManager.m_Fog.m_RenderableObject:set_visible(true)	
	m_CharacterManager.m_Fog.m_Off = false
end 

function FogTriggerStay(_Player, _TriggerName)
	--utils_log("stay")
end 

function FogTriggerExit(_Player, _TriggerName)
	--utils_log("exit")
end 