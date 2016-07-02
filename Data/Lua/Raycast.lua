function CheckRaycast(_Player, _Pos)
	_Player.m_RaycastData = RaycastData()
	_Player.m_PhysXManager:raycast(_Pos+_Player.m_CameraController:get_forward(), _Pos+(_Player.m_CameraController:get_forward()*2), 4, _Player.m_RaycastData)
	if _Player.m_RaycastData.actor_name ~= "" then
		TriggerRaycast(_Player, _Pos)
	end
end

function TriggerRaycast(_Player, _Pos)
	if _Player.m_RaycastData.actor_name == "TriggerComoda" then
		RaycastComoda(_Player, _Pos)
	end
end

function FacingRaycast(_Player, _Pos, _Distance)
	return ((_Pos - l_Target - _Player.m_TargetOffset):length() < _Distance)
end

function RaycastComoda(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerComoda"):get_position()
	if FacingRaycast(_Player, _Pos, 0.75) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "OpenBureau"
		_Player.m_CameraAnimation = _Player.m_InteractingCinematic
		_Player.m_ItemName = "LlaveRecibidor"
		_Player.m_ItemTime = 1.5
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end
