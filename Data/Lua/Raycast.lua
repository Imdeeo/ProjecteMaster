function CheckRaycast(_Player, _Pos)
utils_log("CheckRaycast!")
	_Player.m_RaycastData = RaycastData()
	_Player.m_PhysXManager:raycast(_Pos+_Player.m_CameraController:get_forward(), _Pos+(_Player.m_CameraController:get_forward()*2), 4, _Player.m_RaycastData)
	if _Player.m_RaycastData.actor_name ~= "" then
		TriggerRaycast(_Player, _Pos)
	end
end

function TriggerRaycast(_Player, _Pos)
utils_log("TriggerRaycast!")
	if _Player.m_RaycastData.actor_name == "TriggerComoda" then
		RaycastComoda(_Player, _Pos)
	elseif _Player.m_RaycastData.actor_name == "TriggerDoor" then
		RaycastDoor(_Player, _Pos)
	end
end

function FacingRaycast(_Player, _Target, _Pos, _Distance)
	utils_log("Pos: ".._Pos.x..", ".._Pos.y..", ".._Pos.z)
	utils_log("Target: ".._Target.x..", ".._Target.y..", ".._Target.z)
	utils_log("Offset: ".._Player.m_TargetOffset.x..", ".._Player.m_TargetOffset.y..", ".._Player.m_TargetOffset.z)
	utils_log("Distance: "..(_Pos - _Target - _Player.m_TargetOffset):length())
	return ((_Pos - _Target - _Player.m_TargetOffset):length() < _Distance)
end

function RaycastComoda(_Player, _Pos)
utils_log("RaycastComoda!")
	_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerComoda"):get_position()
	if FacingRaycast(_Player, l_Target, _Pos, 0.75) then
	utils_log("facing!")
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

function RaycastDoor(_Player, _Pos)
	utils_log("RaycastDoor!")
	_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerDoor"):get_position()
	if FacingRaycast(_Player, l_Target, _Pos, 1.75) then
	utils_log("facing!")
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 5
		_Player.m_InteractingCinematic = "CrossDoor"
		_Player.m_CameraAnimation = _Player.m_InteractingCinematic
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		--CUABEngine.get_instance():get_level_manager():load_level("Biblioteca")
	end
end
