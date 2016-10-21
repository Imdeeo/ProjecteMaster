dofile("Data\\Lua\\Raycast\\Recibidor.lua")
dofile("Data\\Lua\\Raycast\\Biblioteca.lua")
dofile("Data\\Lua\\Raycast\\Maquinas.lua")
dofile("Data\\Lua\\Raycast\\TriggerTest.lua")
-- Distance conversion rates:
-- 1,5834710743801652892561983471074(Max) -> 1,0(Engine)
-- 1,0(Max) -> 0,63152400835073068893528183716077(Engine)

function CheckRaycast(_Player, _Camera, _Owner)
	_Player.m_RaycastData = RaycastData()
	
	local didHit = _Player.m_PhysXManager:raycast(_Camera:get_position(), _Camera:get_position()+(_Camera:get_forward()*1.7), 4, _Player.m_RaycastData)
	local l_LayerManager = g_Engine:get_level_manager():get_level(_Player.m_ActualLevel):get_layer_manager()

	if didHit then
		local l_Trigger = l_LayerManager:get_layer("triggers"):get_resource(_Player.m_RaycastData.actor_name)
		if l_LayerManager:get_layer("interactuable_objects"):get_resource(l_Trigger:get_interactuable_object_name()) == nil then
			local l_InteractuableObject = l_LayerManager:get_layer("solid"):get_resource(l_Trigger:get_interactuable_object_name())
			l_LayerManager:get_layer("interactuable_objects"):add_resource(l_InteractuableObject.name, l_InteractuableObject)
		end
		if _Player.m_InputManager:is_action_active("Interact") then
			TriggerRaycast(_Player, _Owner)
		end
	else
		l_LayerManager:get_layer("interactuable_objects"):destroy(false)
	end
end

function TriggerRaycast(_Player, _Owner)
	--utils_log("triggers_raycast")
	l_LevelID = _Player.m_ActualLevel
	if l_LevelID == "Recibidor" then
		if _Player.m_RaycastData.actor_name == "TriggerDoor" then
			if _Player.m_ItemName == "LlaveRecibidor" then
				R1Door(_Player)
			else
				_Player.m_ItemName = "LlaveRecibidor"
				_Player.m_Item = CUABEngine.get_instance():get_level_manager():get_level(_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource(_Player.m_ItemName)
			end
		elseif _Player.m_RaycastData.actor_name == "TriggerSheets" then
			R1Sheets(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayR2" then
			R1TrayR2(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerCanvas" then
			R1Canvas(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayR1" then
			R1TrayR1(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayL2" then
			R1TrayL2(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayL1" then
			R1TrayL1(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerAnkh" then
			R1Ankh(_Player)
		end
	elseif l_LevelID == "Biblioteca" then
		if _Player.m_RaycastData.actor_name == "TriggerOrganKeyA" then
			R2TriggerOrganKeyA(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyB" then
			R2TriggerOrganKeyB(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyC" then
			R2TriggerOrganKeyC(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyD" then
			R2TriggerOrganKeyD(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyE" then
			R2TriggerOrganKeyE(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyF" then
			R2TriggerOrganKeyF(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyG" then
			R2TriggerOrganKeyG(_Player, _Owner)
		elseif _Player.m_RaycastData.actor_name == "TriggerPipeOrgan" then
			R2TriggerPipeOrgan(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerArtifact" then
			R2Artifact(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerArtifactDoor" and _Player.m_ItemName == "Artilufacto" then
			R2ArtifactDoor(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerClue"  and _Player.m_Item == nil then
			R2Clue(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerBook" then
			R2Book(_Player)
		end
	elseif l_LevelID == "Maquinas" then
		if _Player.m_RaycastData.actor_name == "TriggerValve" then
			R3Valve(_Player)
		elseif _Player.m_RaycastData.actor_name == "TriggerValveInput" then
			if _Player.m_ItemName == "ValvulaPuzzle" then
				R3ValveInput(_Player)
			else
				_Player.m_ItemName = "ValvulaPuzzle"
				_Player.m_Item = CUABEngine.get_instance():get_level_manager():get_level(_Player.m_ActualLevel):get_layer_manager():get_resource("solid"):get_resource(_Player.m_ItemName)
			end
		elseif _Player.m_RaycastData.actor_name == "TriggerValveDoor" then
			if R3ValveIsPlaced then
				R3ValveDoor(_Player)
			end
		end
	end
end
