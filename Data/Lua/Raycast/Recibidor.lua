function FacingRaycast(_Offset, _Target, _Pos, _Distance)
	--utils_log("Pos: ".._Pos.x..", ".._Pos.y..", ".._Pos.z)
	--utils_log("Target: ".._Target.x..", ".._Target.y..", ".._Target.z)
	--utils_log("Offset: ".._Offset.x..", ".._Offset.y..", ".._Offset.z)
	--utils_log("Distance: "..(_Pos - _Target):length())
	return ((_Pos - _Target):length() < _Distance)
end

R1TrayR1IsClosed = true
R1TrayR2IsClosed = true
R1TrayL1IsClosed = true
R1TrayL2IsClosed = true

function R1Door(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(0.0, 0.0, -1.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerDoor"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 1
		_Player.m_InteractingCinematic = "CrossDoor"
		_Player.m_CameraAnimation = "CrossDoor"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1
		_Player.m_CurrentAend = "CrossDoor"
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		--CUABEngine.get_instance():get_level_manager():load_level("Biblioteca")
	end
end

function R1Sheets(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(0.5, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerSheets"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 0.5) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Sheets->")
	end
end

function R1Canvas(_Player, _Pos)
utils_log("R1Canvas")
	_Player.m_TargetOffset = Vect3f(-1.5, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerCanvas"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 2.0) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Canvas->")
	end
end

function R1TrayR1(_Player, _Pos) --This contains the key
	if R1TrayR1IsClosed then
		_Player.m_TargetOffset = Vect3f(0.4, 0.0, 0.0)
		l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayR1"):get_position()
		if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
			_Player.m_Target = l_Target
			_Player.m_InteractingAnimation = 4
			_Player.m_InteractingCinematic = "ForceTray"
			_Player.m_CameraAnimation = "ForceTray"
			_Player.m_NewItemName = "LlaveRecibidor"
			_Player.m_ItemTime = 1
			_Player.m_CurrentAend = nil
			_Player.m_IsInteracting = true
			_Player.m_IsClimbing = false
			_Player.m_IsCorrecting = true
			R1TrayR1IsClosed = false
		end
	end
end

function R1TrayR2Open(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayR2"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayR2"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayR2IsClosed = false
	end
end

function R1TrayR2Close(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayR2"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayR2"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayR2IsClosed = true
	end
end

function R1TrayR2(_Player, _Pos)
	if R1TrayR2IsClosed then
		R1TrayR2Open(_Player, _Pos)
	else
		R1TrayR2Close(_Player, _Pos)
	end
end

function R1TrayL1Open(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL1"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL1"
		_Player.m_CameraAnimation = "OpenTrayInverse"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayL1IsClosed = false
	end
end

function R1TrayL1Close(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL1"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL1"
		_Player.m_CameraAnimation = "CloseTrayInverse"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayL1IsClosed = true
	end
end

function R1TrayL1(_Player, _Pos)
	if R1TrayL1IsClosed then
		R1TrayL1Open(_Player, _Pos)
	else
		R1TrayL1Close(_Player, _Pos)
	end
end

function R1TrayL2Open(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL2"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL2"
		_Player.m_CameraAnimation = "OpenTrayInverse"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayL2IsClosed = false
	end
end

function R1TrayL2Close(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-0.4, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL2"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL2"
		_Player.m_CameraAnimation = "CloseTrayInverse"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		R1TrayL2IsClosed = true
	end
end

function R1TrayL2(_Player, _Pos) --This contains the ankh
	if R1TrayL2IsClosed then
		R1TrayL2Open(_Player, _Pos)
	else
		R1TrayL2Close(_Player, _Pos)
	end
end

function R1Ankh(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerAnkh"):get_position()
	if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Ankh->")
	end
end

function R1Key(_Player, _Pos)
	utils_log("<-Interacted with R1Key->")
	--_Player.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
	--l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerKey"):get_position()
	--if FacingRaycast(_Player.m_TargetOffset, l_Target, _Pos, 1.4) then
	--	_Player.m_Target = l_Target
	--	_Player.m_InteractingAnimation = 1
	--	_Player.m_InteractingCinematic = "PickKey"
	--	_Player.m_CameraAnimation = "PickKey"
	--	_Player.m_NewItemName = "LlaveRecibidor"
	--	_Player.m_ItemTime = 1.5
	--	_Player.m_AnimationTime = 2.6
	--	_Player.m_CurrentAend = nil
	--	_Player.m_IsInteracting = true
	--	_Player.m_IsClimbing = false
	--	_Player.m_IsCorrecting = true
	--end
end
