dofile("Data\\Lua\\Player\\Helpers.lua")

R3ValveIsPlaced = false

local l_LevelId = "Maquinas"

function R3Valve(_Player)
	_Player.m_TargetYaw = g_PI
	_Player.m_ForwardCamera = Vect3f(0.766055, -0.64279, 0.0)
	_Player.m_UpCamera = Vect3f(0.642787, 0.766045, 0.0)
	_Player.m_TargetPosOffset = Vect3f(-0.86, 0.0, 0.004)
	l_Target = GetObjectPos("ValvulaPuzzle", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 2.0) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 24
		_Player.m_InteractingCinematic = nil
		_Player.m_AnimationTime = 4.66667
		_Player.m_CameraAnimation = "PickValve"
		_Player.m_NewItemName = "ValvulaPuzzle"
		_Player.m_ItemTime = 3.33333
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = true
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_PhysXManager:enable_trigger("TriggerValveInput", "FisicasAux")
		_Player.m_PhysXManager:disable_physics("TriggerValve", "FisicasAux")
		--Play Sound¿?
	end
end

function R3ValveInput(_Player)
	_Player.m_TargetYaw = -g_PI/4.0
	_Player.m_ForwardCamera = Vect3f(-0.5, 0.0, 0.866)
	_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
	l_Target = GetTriggerPos("TriggerValveInput", l_LevelId)
	if _Player.m_PhysXManager:get_character_controler_pos("player").x >= l_Target.x+0.5061 then
		_Player.m_TargetPosOffset = Vect3f(0.5057, 0.0, -0.6928)
	else
		_Player.m_TargetPosOffset = Vect3f(0.5065, 0.0, -0.6928)
	end
	if _Player:IsFacingTarget(l_Target, 1.0, 1.2) then
		_Player.m_Target = l_Target
		_Player.m_InteractingAnimation = 25
		_Player.m_InteractingCinematic = "InputValve"
		_Player.m_AnimationTime = 6.666667
		_Player.m_CameraAnimation = "InputValve"
		_Player.m_NewItemName = ""
		_Player.m_ItemTime = 1.233333
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_LeftHanded = true
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_FogDown= true
		_Player.m_BiasPos = 0.02
		-- Play Sound
		R3ValveIsPlaced = true
		_Player.m_CinematicManager:get_resource("ResolveValve"):play()
		_Player.m_PhysXManager:disable_physics("Maquinas_CajaEngranaje", "FisicasAux")
		_Player.m_PhysXManager:enable_object("Maquinas_CajaEngranajeMovido", "FisicasAux")
		_Player.m_PhysXManager:disable_physics("TriggerValveInput", "FisicasAux")
		local l_Level = g_Engine:get_level_manager():get_level(l_LevelId)
		local l_CinematiManager = l_Level:get_cinematic_manager()
		l_CinematiManager:get_resource("21gramos"):play()
		l_CinematiManager:get_resource("TrituradoraPared"):play()
		l_CinematiManager:get_resource("TrituradoraTecho"):play()
		l_CinematiManager:get_resource("Newcommen"):play()
		
		local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
		local l_Particle = l_Level:get_layer_manager():get_layer("particles"):get_resource("EmisorPipeSteam")
		
		l_Particle:set_start(true)
		l_Particle:set_visible(true)	
		
		
		l_Particle = l_Level:get_layer_manager():get_layer("particles"):get_resource("ValveSteam")
		l_Particle:set_awake(false)
		l_Particle:set_awake_timer(10000)
		
		m_CharacterManager.m_EnemicsMap["Maquinas"]["FogAutomaton"].m_Awake = false
	end
end

function R3ValveDoor(_Player)
	local l_LevelManager = CUABEngine.get_instance():get_level_manager()
	l_LevelManager:get_level("Boss"):set_has_to_update(true)
	l_LevelManager:get_level("Boss"):set_visible(true)
	_Player.m_TargetYaw = 0.0
	_Player.m_TargetPosOffset = Vect3f(0.601, 0.0, -1.397)
	_Player.m_ForwardCamera = Vect3f(-0.866025, -0.5, 0.0)
	_Player.m_UpCamera = Vect3f(0.5, 0.866026, 0.0)
	l_Target = GetTriggerPos("TriggerValveDoor", l_LevelId)
	if _Player:IsFacingTarget(l_Target, 1.0, 1.0) then
		_Player.m_Target = Vect3f(-1.06923, -0.691453, 40.8816)
		_Player.m_InteractingAnimation = 26
		_Player.m_InteractingCinematic = "CrossValveDoor"
		_Player.m_CameraAnimation = "CrossValveDoor"
		_Player.m_ItemDropTime = -1.0
		_Player.m_CurrentAend = nil
		_Player.m_IsInteracting = true
		_Player.m_IsCorrecting = true
		_Player.m_IsPuzzle = false
		_Player.m_Teleport = true
		_Player.m_BiasPos=0.04
		-- Play Sound
	end
end
