dofile("Data\\Lua\\Raycast\\Helpers.lua")

R1TrayR1IsClosed = true
R1TrayR2IsClosed = true
R1TrayL1IsClosed = true
R1TrayL2IsClosed = true

function R1Door(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.048, 0.0, -0.55)
	l_Target = GetTriggerPos("TriggerDoor")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 1
		_Player.m_InteractingCinematic = "CrossDoor"
		_Player.m_CameraAnimation = "CrossDoor"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.5
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = "CrossDoor"
		_Player.m_LeftHanded = false
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
	end
end

function R1Sheets(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.5, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerSheets")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 0.5) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Sheets->")
	end
end

function R1Canvas(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-1.5, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerCanvas")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 2.0) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Canvas->")
	end
end

function R1TrayR1(_Player, _Pos) --This contains the key
<<<<<<< HEAD
	if R1TrayR1IsClosed then
		_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
		_Player.m_TargetPosOffset = Vect3f(0.728, 0.0, 0.03958677)
		l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayR1"):get_position()
		if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
			_Player.m_Target = l_Target
			_Player.m_InteractingAnimation = 4
			_Player.m_InteractingCinematic = "ForceTray"
			_Player.m_CameraAnimation = "ForceTray"
			_Player.m_NewItemName = "LlaveRecibidor"
			_Player.m_ItemTime = 4.06666
			_Player.m_LeftHanded = true
			_Player.m_CurrentAend = nil
			_Player.m_IsInteracting = true
			_Player.m_IsClimbing = false
			_Player.m_IsCorrecting = true
			R1TrayR1IsClosed = false
			--m_CharacterManager.m_Enemics[1].m_Awake = true
			--m_CharacterManager.m_Enemics[2].m_Awake = true
		end
=======
	_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.728, 0.0, 0.03958677)
	l_Target = GetTriggerPos("TriggerTrayR1")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 4
		_Player.m_InteractingCinematic = "ForceTray"
		_Player.m_CameraAnimation = "ForceTray"
		_Player.m_NewItemName = "LlaveRecibidor"
		_Player.m_ItemTime = 4.06666
		_Player.m_ItemDropTime = -1.0
		_Player.m_LeftHanded = true
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		--m_CharacterManager.m_Enemics[1].m_Awake = true
		--m_CharacterManager.m_Enemics[2].m_Awake = true
		g_Engine:get_cinematic_manager():get_resource("recibidor_engranajes"):play()
		_Player.m_PhysXManager:disable_trigger("TriggerTrayR1")
>>>>>>> develop
	end
end

function R1TrayR2Open(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.705, 0.0, 0.03958677)
	l_Target = GetTriggerPos("TriggerTrayR2")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayR2"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayR2IsClosed = false
	end
end

function R1TrayR2Close(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.705, 0.0, 0.03958677)
	l_Target = GetTriggerPos("TriggerTrayR2")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayR2"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
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
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.705, 0.0, -0.03958677)
	l_Target = GetTriggerPos("TriggerTrayL1")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL1"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayL1IsClosed = false
	end
end

function R1TrayL1Close(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.705, 0.0, -0.03958677)
	l_Target = GetTriggerPos("TriggerTrayL1")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL1"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
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
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.705, 0.0, -0.03958677)
	l_Target = GetTriggerPos("TriggerTrayL2")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL2"):set_scale(Vect3f(1.0, 0.5, 1.0))
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
		_Player.m_InteractingCinematic = "OpenTrayL2"
		_Player.m_CameraAnimation = "OpenTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		R1TrayL2IsClosed = false
	end
end

function R1TrayL2Close(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.705, 0.0, -0.03958677)
	l_Target = GetTriggerPos("TriggerTrayL2")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerTrayL2"):set_scale(Vect3f(1.0, 1.0, 1.0))
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 3
		_Player.m_InteractingCinematic = "CloseTrayL2"
		_Player.m_CameraAnimation = "CloseTray"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
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
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerAnkh")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
		_Player.m_Target = l_Target
		-- Play Sound
		utils_log("<-Interacted with R1Ankh->")
	end
end

function R1Key(_Player, _Pos)
	utils_log("<-Interacted with R1Key->")
	--_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	--l_Target = GetTriggerPos("TriggerKey")
	--if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.4) then
	--	_Player.m_Target = l_Target
	--	_Player.m_InteractingAnimation = 1
	--	_Player.m_InteractingCinematic = "PickKey"
	--	_Player.m_CameraAnimation = "PickKey"
	--	_Player.m_NewItemName = "LlaveRecibidor"
	--	_Player.m_ItemTime = 1.5
	--	_Player.m_ItemDropTime = -1.0
	--	_Player.m_AnimationTime = 2.6
	--	_Player.m_CurrentAend = nil
	--	_Player.m_IsInteracting = true
	--	_Player.m_IsClimbing = false
	--	_Player.m_IsCorrecting = true
	--	_Player.m_IsPuzzle = false
	--end
end
