function R1Door(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(0.0, 0.0, -1.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerDoor"):get_position()
	if FacingRaycast(_Player, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 2
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

end

function R1TrayR2(_Player, _Pos)

end

function R1Canvas(_Player, _Pos)

end

function R1TrayR1(_Player, _Pos)

end

function R1TrayL2(_Player, _Pos)

end

function R1TrayL1(_Player, _Pos)

end

function R1Ankh(_Player, _Pos)

end

function R1Key(_Player, _Pos)
	_Player.m_TargetOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerComoda"):get_position()
	if FacingRaycast(_Player, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 1
		_Player.m_InteractingCinematic = "PickKey"
		_Player.m_CameraAnimation = "PickKey"
		_Player.m_NewItemName = "LlaveRecibidor"
		_Player.m_ItemTime = 1.5
		_Player.m_AnimationTime = 2.6
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end