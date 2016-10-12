dofile("Data\\Lua\\Raycast\\PipeOrgan.lua")
dofile("Data\\Lua\\Raycast\\Helpers.lua")

function R2TriggerOrganKeyA(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(10, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("A", _Player)
end

function R2TriggerOrganKeyB(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(11, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("B", _Player)
end

function R2TriggerOrganKeyC(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(12, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("C", _Player)
end

function R2TriggerOrganKeyD(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(13, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("D", _Player)
end

function R2TriggerOrganKeyE(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(14, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("E", _Player)
end

function R2TriggerOrganKeyF(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(15, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("F", _Player)
end

function R2TriggerOrganKeyG(_Player, _Owner)
	--play key sound
	_Owner:remove_action(_Owner:get_actual_action_animation())
	_Owner:execute_action(16, 0.1, 0.1, 1.0, true)
	R2PushOrganKey("G", _Player)
end

function R2TriggerPipeOrgan(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.0, 0.0, -1.0)
	l_Target = GetTriggerPos("TriggerPipeOrgan")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.8) then
		_Player.m_Target = l_Target
		_Player.m_AnimationTime = 0.6667
		_Player.m_InteractingAnimation = 8
		_Player.m_InteractingCinematic = nil
		_Player.m_CameraAnimation = "PipeOrganStart"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = true
		_Player.m_PhysXManager:disable_trigger("TriggerPipeOrgan")
	end
end

function R2Artifact(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.0, 0.0, -0.6)
	l_Target = GetTriggerPos("TriggerArtifact")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.8) then
		_Player.m_Target = l_Target
		_Player.m_AnimationTime = 5
		_Player.m_LeftHanded = true
		_Player.m_InteractingAnimation = 6
		_Player.m_InteractingCinematic = nil
		_Player.m_CameraAnimation = "PickArtifact"
		_Player.m_NewItemName = "Artilufacto"
		_Player.m_ItemTime = 1
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
	end
end

function R2ArtifactDoor(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.5, 0.0, -0.5)
	l_Target = GetTriggerPos("TriggerArtifactDoor")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 5
		_Player.m_InteractingCinematic = "CrossArtifactDoor"
		_Player.m_CameraAnimation = "CrossArtifactDoor"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.6667
		_Player.m_ItemDropTime = -1.0
		--_Player.m_CurrentAend = "CrossArtifactDoor"
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
	end
end

function R2Clue(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.86, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerClue")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.6) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 7
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 7
		_Player.m_CameraAnimation = "PickClue"
		_Player.m_NewItemName = "PistaBiblio"
		_Player.m_LeftHanded = true
		_Player.m_ItemTime = 1
		_Player.m_ItemDropTime = 6.5
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
	end
end

function R2Book(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, -1.0)
	_Player.m_TargetPosOffset = Vect3f(0.0, 0.0, -0.86)
	l_Target = GetTriggerPos("TriggerBook")
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.8) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 17
		_Player.m_InteractingCinematic = "PullBook"
		_Player.m_CameraAnimation = "PullBook"
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsClimbing = false
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
	end
	-- activate projector -> launch cinematic, activate lights and particles
end