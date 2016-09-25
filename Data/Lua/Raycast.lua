dofile("Data\\Lua\\Raycast\\Recibidor.lua")
dofile("Data\\Lua\\Raycast\\Biblioteca.lua")
-- Distance conversion rates:
-- 1,5834710743801652892561983471074(Max) -> 1,0(Engine)
-- 1,0(Max) -> 0,63152400835073068893528183716077(Engine)

function CheckRaycast(_Player, _Pos)
	_Player.m_RaycastData = RaycastData()
	_Player.m_PhysXManager:raycast(_Player.m_CameraController:get_position(), _Player.m_CameraController:get_position()+(_Player.m_CameraController:get_forward()*1.7), 4, _Player.m_RaycastData)
	--utils_log(_Player.m_RaycastData.actor_name)
	if _Player.m_RaycastData.actor_name ~= "" then
		TriggerRaycast(_Player, _Pos)
	end
end

function TriggerRaycast(_Player, _Pos)
	l_LevelID = g_Engine:get_level_loaded()
	if l_LevelID == "1" then
		if _Player.m_RaycastData.actor_name == "TriggerDoor" then
			if _Player.m_ItemName == "LlaveRecibidor" then
				R1Door(_Player, _Pos)
			end
		elseif _Player.m_RaycastData.actor_name == "TriggerSheets" then
			R1Sheets(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayR2" then
			R1TrayR2(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerCanvas" then
			R1Canvas(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayR1" then
			R1TrayR1(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayL2" then
			R1TrayL2(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerTrayL1" then
			R1TrayL1(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerAnkh" then
			R1Ankh(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerKey" then
			if _Player.m_ItemName == "" then
				R1Key(_Player, _Pos)
			end
		end
	elseif l_LevelID == "2" then
	utils_log("Trigger: ".._Player.m_RaycastData.actor_name)
		if _Player.m_RaycastData.actor_name == "TriggerOrganKeyA" then
			R2TriggerOrganKeyA(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyB" then
			R2TriggerOrganKeyB(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyC" then
			R2TriggerOrganKeyC(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyD" then
			R2TriggerOrganKeyD(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyE" then
			R2TriggerOrganKeyE(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerOrganKeyF" then
			R2TriggerOrganKeyF(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerArtifact" then
			if R2PuzzleCleared then
				R2Artifact(_Player, _Pos)
			end
		elseif _Player.m_RaycastData.actor_name == "TriggerArtifactInput" then
			R2ArtifactInput(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerArtifactDoor" then
			if _Player.m_ItemName == "Artilufacto" then
				R2ArtifactDoor(_Player, _Pos)
			end
		elseif _Player.m_RaycastData.actor_name == "TriggerClue" then
			R2Clue(_Player, _Pos)
		elseif _Player.m_RaycastData.actor_name == "TriggerBook" then
			R2Book(_Player, _Pos)
		end
	end
end
