dofile("Data\\Lua\\Raycast\\Recibidor.lua")

function CheckRaycast(_Player, _Pos)
	_Player.m_RaycastData = RaycastData()
	_Player.m_PhysXManager:raycast(_Player.m_CameraController:get_position(), _Player.m_CameraController:get_position()+(_Player.m_CameraController:get_forward()*1.7), 4, _Player.m_RaycastData)
	--utils_log(_Player.m_RaycastData.actor_name)
	if _Player.m_RaycastData.actor_name ~= "" then
		TriggerRaycast(_Player, _Pos)
	end
end

function TriggerRaycast(_Player, _Pos)
	--utils_log(_Player.m_ItemName)
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
	end
end
