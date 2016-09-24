function FacingRaycast(_Offset, _Target, _Pos, _Distance)
	--utils_log("Pos: ".._Pos.x..", ".._Pos.y..", ".._Pos.z)
	--utils_log("Target: ".._Target.x..", ".._Target.y..", ".._Target.z)
	--utils_log("Offset: ".._Offset.x..", ".._Offset.y..", ".._Offset.z)
	--utils_log("Distance: "..(_Pos - _Target):length())
	return ((_Pos - _Target):length() < _Distance)
end

R2PuzzleCleared = false

function R2PipeOrgan(_Player, _Pos)
	
end

function R2Artifact(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(-0.86, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerArtifact"):get_position()
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.6) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 6
		_Player.m_InteractingCinematic = "PickArtifact"
		_Player.m_CameraAnimation = "PickArtifact"
		_Player.m_NewItemName = "Artilufacto"
		_Player.m_ItemTime = 1
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end

function R2ArtifactDoor(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.0, 0.0, -0.50143)
	l_Target = Vect3f(0.0, 0.0, 0.0)
	l_Target.x = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerArtifactDoor"):get_position().x
	l_Target.z = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerArtifactDoor"):get_position().z
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 5
		_Player.m_InteractingCinematic = "CrossArtifactDoor"
		_Player.m_CameraAnimation = "CrossArtifactDoor"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.6667
		_Player.m_CurrentAend = "CrossArtifactDoor"
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end

function R2Clue(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.86, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerClue"):get_position()
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.6) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 7
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 7
		_Player.m_CameraAnimation = "PickClue"
		_Player.m_NewItemName = "hojaspapel"
		_Player.m_ItemTime = 1.1
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end

function R2Book(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(-0.86, 0.0, 0.0)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource("TriggerBook"):get_position()
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.6) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 7
		_Player.m_InteractingCinematic = "PullBook"
		_Player.m_CameraAnimation = "PullBook"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
	end
end