dofile("Data\\Lua\\Player\\Helpers.lua")

function OnEnterWindow(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		l_Player.m_CameraAnimation = "Window"
		l_Player.m_AnimationTime = 3.0
		l_Player.m_TargetSanity = 20.0
		l_Player.m_IsFocusing = true
		l_Player.m_TargetOffset = Vect3f(-2.65267, 5.04859, 32.6297)
		l_Player.m_PhysXManager:disable_physics(_TriggerName, "FisicasAux")		
		l_Player.m_PhysXManager:disable_physics("Maquinas_fisica_escaleras", "FisicasAux")
	end
end

function OnExitWindow(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		l_Player.m_PhysXManager:disable_physics(_TriggerName, "FisicasAux")
	end
end

function OnEnterStairs(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		if not l_Player.m_IsClimbing and l_Player.m_Item == nil then
			l_Player.m_InteractingCinematic = nil
			l_Player.m_CameraAnimation = nil
			l_Player.m_AnimationTime = 0.666667
			l_Player.m_InteractingAnimation = 18
			l_Player.m_IsClimbing = true
			
			utils_log("LEVEL: "..l_Player.m_ActualLevel)
			if l_Player.m_ActualLevel == "Maquinas" then
				l_Player.m_ForwardCamera = Vect3f(-1.4142135623730950488016887242097, 0.0, -1.4142135623730950488016887242097)
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("Escalera"):get_position()
				l_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
				l_Player.m_ForwardCamera = Vect3f(-0.5, 0.0, -0.5)
				if _TriggerName == "TriggerStairsUpper" then
					l_Player.m_TargetPosOffset = Vect3f(0.46, 1.55, 0.46)
				else
					l_Player.m_TargetPosOffset = Vect3f(0.45, -l_Player.m_Target.y, 0.45)
				end
			elseif l_Player.m_ActualLevel == "Boss" then
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("EscaleraSalaBoss"):get_position()
				l_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
				l_Player.m_ForwardCamera = Vect3f(-1.0, 0.0, 0.0)
				l_Player.m_TargetPosOffset = Vect3f(0.8, 0.0, 0.0)
			elseif l_Player.m_ActualLevel == "Pasillo" then
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("EscaleraEscape"):get_position()
				l_Player.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
				l_Player.m_ForwardCamera = Vect3f(1.0, 0.0, 0.0)
				l_Player.m_TargetPosOffset = Vect3f(-0.5, 0.0, 0.0)
			end				
		end
	end
end

function OnExitStairs(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
	local l_Player = m_CharacterManager.m_Player[1]
		if not l_Player.m_IsClimbing then
			l_Player:ClearTarget()
			l_Player:ClearStates()
		end

		if l_Player.m_ActualLevel == "Maquinas" then
			if _TriggerName == "TriggerStairsLower" then
				local l_FogMaterial = g_Engine:get_level_manager():get_level("Maquinas"):get_material_manager():get_resource("FogMaterial")		
				if l_FogMaterial:get_value(0) == 1 then
					m_CharacterManager.m_EnemicsMap["Maquinas"]["FogAutomaton"].m_Awake = true
				end
			elseif _TriggerName == "TriggerStairsUpper" then
				local l_FogMaterial = g_Engine:get_level_manager():get_level("Maquinas"):get_material_manager():get_resource("FogMaterial")		
				if l_FogMaterial:get_value(0) == 1 then
					m_CharacterManager.m_EnemicsMap["Maquinas"]["FogAutomaton"].m_Awake = false
				end
			end
		end
	end
end

function OnStayStairsLower(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		if l_Player.m_ClimbingDown then			
			l_Player.m_CurrentAend = nil
			l_Player.m_IsClimbing = false
			l_Player.m_InteractingAnimation = 21
			l_Player.m_InteractingCinematic = nil
			l_Player.m_CameraAnimation = nil
			l_Player.m_AnimationTime = 1.5
		end
	end
end

function OnStayStairsUpper(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		if l_Player.m_ClimbingUp then
			l_Player.m_CurrentAend = "LeaveStairs"
			l_Player.m_InteractingAnimation = 0
			l_Player.m_InteractingCinematic = nil
			l_Player.m_CameraAnimation = nil--"LeaveStairs"
			l_Player.m_AnimationTime = 1.0
			l_Player.m_IsInteracting = true
			l_Player.m_IsClimbing = false
			
			if g_Player.m_ActualLevel == "Maquinas" then

			end
		end
	end
end

function OnEnterStairsLower(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		l_Player.m_TargetLookOffset = Vect3f(-1.4142135623730950488016887242097, 0.0, -1.4142135623730950488016887242097)
		l_Player.m_TargetPosOffset = Vect3f(0.4, 0.0, 0.4)
		l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("Escalera"):get_position()
		l_Player.m_IsClimbing = not l_Player.m_IsClimbing		
	end
end

function CheckIfFacing(_Player, _TriggerName)
	l_Target = CUABEngine.get_instance():get_layer_manager():get_resource("triggers"):get_resource(_TriggerName):get_position()
	--// Check if angle between player camera forward and trigger offset is 180+- 65 degrees (<2.0 rads)
	--// Then checks if angle between the vector triggerPos->playerPos and offset is less than 90 degrees (1.57 rads)
	if (_Player.m_CameraController:get_forward():get_normalized(1):get_angle_with(_Player.m_TargetOffset) > 2.0 and (_Player.m_PhysXManager:get_character_controler_pos("player") - l_Target):get_normalized(1):get_angle_with(_Player.m_TargetOffset) < 1.57) then
		_Player.m_Target = l_Target
		_Player.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
		_Player.m_IsInteracting = false
		_Player.m_IsClimbing = true
		_Player.m_IsCorrecting = true
	end
end

function FogTriggerEnter(_Player, _TriggerName)
	g_Engine:get_physX_manager():character_controller_teleport(m_CharacterManager.m_Fog.m_Name, Vect3f(8.4,0, 3.9))
	m_CharacterManager.m_Fog.m_RenderableObject:set_visible(true)	
	m_CharacterManager.m_Fog.m_Off = false
end 

function FogTriggerStay(_Player, _TriggerName)
	--utils_log("stay")
end 

function FogTriggerExit(_Player, _TriggerName)
	--utils_log("exit")
end 

function OnEnterHole(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		l_Player:ModifySanity(-100)
	end
end

function OnEnterActivateBoss(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Enemy = m_CharacterManager.m_EnemicsMap["Pasillo"]["Boss"]
		l_Enemy.m_PhysXManager:character_controller_teleport("Boss", Vect3f(2.821, -5.896054, 20.682))
		l_Enemy.m_RenderableObject:set_position(l_Enemy.m_PhysXManager:get_character_controler_pos("Boss"))
		l_Enemy.m_Awake = true
	end
end

function OnExitActivateBoss(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		g_Engine:get_level_manager():choose_scene_command_level("Pasillo")
		g_Player:SetActualLevel("Pasillo")
	end
end