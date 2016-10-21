dofile("Data\\Lua\\Player\\Helpers.lua")

R3ValveIsPlaced = false

local l_LevelId = "Maquinas"

function R3Valve(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(1.0, 0.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-1.0, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerValve", l_LevelId)
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 2.0) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 23
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 7.3333333
		_Player.m_CameraAnimation = nil--"PickValve"
		_Player.m_NewItemName = "ValvulaPuzzle"
		_Player.m_ItemTime = 5.0
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = false
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		--Play Sound¿?
	end
end

function R3ValveInput(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 1.0)
	_Player.m_TargetPosOffset = Vect3f(1.0, 0.0, -1.0)
	l_Target = GetTriggerPos("TriggerValveInput", l_LevelId)
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 24
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 6.666667
		_Player.m_CameraAnimation = nil--"InputValve"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 4.666667
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = true
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		-- Play Sound
		R3ValveIsPlaced = true
	end
end

function R3ValveDoor(_Player, _Pos)
	_Player.m_TargetLookOffset = Vect3f(-1.0, 0.0, 1.0)
	_Player.m_TargetPosOffset = Vect3f(-1.0, 0.0, 1.0)
	l_Target = GetTriggerPos("TriggerValveDoor", l_LevelId)
	if FacingRaycast(_Player.m_TargetLookOffset, l_Target, _Pos, 1.0) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 25
		_Player.m_InteractingCinematic = "CrossValveDoor"
		_Player.m_CameraAnimation = "CrossValveDoor"
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = "CrossValveDoor"
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		-- Play Sound
	end
end
