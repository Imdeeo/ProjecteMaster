dofile("Data\\Lua\\Player\\Helpers.lua")

R3ValveIsPlaced = false

local l_LevelId = "Maquinas"

function R3Valve(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_ForwardCamera = Vect3f(0.766045, -0.642787, 0.0)
	_Player.m_UpCamera = Vect3f(0.642787, 0.766045, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-1.65, 0.0, 0.0)
	l_Target = GetTriggerPos("TriggerValve", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 2.0) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 24
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 7.3333333
		_Player.m_CameraAnimation = "PickValve"
		_Player.m_NewItemName = "ValvulaPuzzle"
		_Player.m_ItemTime = 5.0
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = true
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		--_Player.m_PhysXManager:enable_trigger("TriggerValveInput")
		--_Player.m_PhysXManager:disable_physics("TriggerValve")
		--Play Sound¿?
	end
end

function R3ValveInput(_Player)
	_Player.m_TargetYaw = -g_PI/4.0
	_Player.m_ForwardCamera = Vect3f(-0.5, 0.0, 0.866)
	_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(0.4, 0.0, -0.6928)
	l_Target = GetTriggerPos("TriggerValveInput", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 25
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
		_Player.m_CinematicManager:get_resource("ResolveValve"):play()
		_Player.m_PhysXManager:disable_physics("CajaEngranaje", "FisicasAux")
		_Player.m_PhysXManager:enable_object("CajaEngranajeMovido", "FisicasAux")
		--_Player.m_PhysXManager:disable_physics("TriggerValveInput")
	end
end

function R3ValveDoor(_Player)
	_Player.m_TargetYaw = -g_PI/2.0
	_Player.m_ForwardCamera = Vect3f(1.0, 0.0, 0.0)
	_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-1.0, 0.0, 1.0)
	l_Target = GetTriggerPos("TriggerValveDoor", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.0) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 26
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
