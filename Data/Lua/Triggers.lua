dofile("Data\\Lua\\Player\\Helpers.lua")

function OnEnterWindow(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		l_Player.m_CameraAnimation = "Window"
		l_Player.m_AnimationTime = 3.0
		l_Player.m_TargetSanity = 20.0
		l_Player.m_IsFocusing = true
		l_Player.m_TargetOffset = Vect3f(0.571789, 6.51367, 5.9193)
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
			
			if l_Player.m_ActualLevel == "Maquinas" then
				l_Player.m_ForwardCamera = Vect3f(-1.4142135623730950488016887242097, 0.0, -1.4142135623730950488016887242097)
				l_Player.m_TargetPosOffset = Vect3f(0.45, 0.0, 0.45)
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("Escalera"):get_position()
			elseif l_Player.m_ActualLevel == "Boss" then
				l_Player.m_ForwardCamera = Vect3f(-1.0, 0.0, 0.0)
				l_Player.m_TargetPosOffset = Vect3f(0.60, 0.0, 0.0)
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("EscaleraBoss"):get_position()
			elseif l_Player.m_ActualLevel == "Pasillo" then
				l_Player.m_ForwardCamera = Vect3f(1.0, 0.0, 0.0)
				l_Player.m_TargetPosOffset = Vect3f(-0.60, 0.0, 0.0)
				l_Player.m_Target = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource("EscaleraEscape"):get_position()
			end			
			
			
		end
	end
end

function OnExitStairs(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
	local l_Player = m_CharacterManager.m_Player[1]
		if not l_Player.m_IsClimbing then
			utils_log("Exit Upper")
			l_Player:ClearTarget()
			l_Player:ClearStates()
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
			
			if g_Player.m_ActualLevel == "Maquinas" then
				local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
				local l_Material = l_Level:get_material_manager():get_resource("FogMaterial")
				l_Material:set_value(1, 1.0)
			end
		end
	end
end

function OnStayStairsUpper(_TriggerName, _ColliderName)
	if(_ColliderName == "player") then
		local l_Player = m_CharacterManager.m_Player[1]
		if l_Player.m_ClimbingUp then
			utils_log("Stay Upper")
			l_Player.m_CurrentAend = "LeaveStairs"
			l_Player.m_InteractingAnimation = 0
			l_Player.m_InteractingCinematic = nil
			l_Player.m_CameraAnimation = nil--"LeaveStairs"
			l_Player.m_AnimationTime = 1.0
			l_Player.m_IsInteracting = true
			l_Player.m_IsClimbing = false
			
			if g_Player.m_ActualLevel == "Maquinas" then
				local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
				local l_Material = l_Level:get_material_manager():get_resource("FogMaterial")
				l_Material:set_value(1, 4.0)
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
		local l_Enemy = m_CharacterManager.m_Enemics[1]
		l_Enemy.m_PhysXManager:character_controller_teleport("Boss", Vect3f(3.821, -4.0, 20.682))
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