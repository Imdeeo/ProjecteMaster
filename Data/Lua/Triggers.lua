function OnChapelEnter(a, b)
	m_CharacterManager.m_Enemics[1].m_off = false
end

function OnChapelExit(a, b)
	m_CharacterManager.m_Enemics[1].m_off = true
end

function OnChapelStay(a, b)

end

function OnEnterOpenShelf(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	self.m_InteractingAnimation = 6
	l_Player.m_IsInteracting = true
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
end

function OnEnterInteracting(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	l_Player.m_IsInteracting = true
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
end

function OnExitInteracting(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	l_Player.m_IsInteracting = false
	l_Player.m_IsCorrecting = false
end

function OnEnterClimbing(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	--TODO: Set l_Player.m_TargetOffset from trigger
	l_Player.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
	l_Player.m_IsInteracting = false
	l_Player.m_IsClimbing = true
	l_Player.m_IsCorrecting = false
end

function OnExitClimbing(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	l_Player.m_IsClimbing = false
	l_Player.m_IsCorrecting = false
end

function OnStayCorrecting(_TriggerName, _ColliderName)
	local l_Player = m_CharacterManager.m_Player[1]
	if (_ColliderName == l_Player.m_Name and l_Player.m_InputManager:is_action_active("INTERACT") and not l_Player.m_IsCorrecting) then
		CheckIfFacing(l_Player, _TriggerName)
	end
end

function CheckIfFacing(_Player, _TriggerName)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource(_TriggerName):get_position()
	-- Check if angle between player camera forward and trigger offset is 180+- 65 degrees (<2.0 rads)
	-- Then checks if angle between the vector triggerPos->playerPos and offset is less than 90 degrees (1.57 rads)
	if (_Player.m_CameraController:get_forward():get_normalized(1):get_angle_with(_Player.m_TargetOffset) > 2.0 and (_Player.m_PhysXManager:get_character_controler_pos("player") - l_Target):get_normalized(1):get_angle_with(_Player.m_TargetOffset) < 1.57) then
		_Player.m_Target = l_Target
		_Player.m_IsCorrecting = true
	end
end