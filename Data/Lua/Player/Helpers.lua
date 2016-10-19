function GetTriggerPos(_Target,_LevelId)
	return CUABEngine.get_instance():get_level_manager():get_level(_LevelId):get_layer_manager():get_resource("triggers"):get_resource(_Target):get_position() - Vect3f(0.0, g_TotalHeight, 0.0)
end

function FacingRaycast(_Offset, _Target, _Pos, _Distance)
	--utils_log("Pos: ".._Pos.x..", ".._Pos.y..", ".._Pos.z)
	--utils_log("Target: ".._Target.x..", ".._Target.y..", ".._Target.z)
	--utils_log("Offset: ".._Offset.x..", ".._Offset.y..", ".._Offset.z)
	--utils_log("Distance: "..(_Pos - _Target):length())
	return ((_Pos - _Target):length() < _Distance)
end

function ClearPlayerTarget(_Player)
	_Player.m_Target = nil
	_Player.m_TargetPosOffset = Vect3f(0.0, 0.0, 0.0)
	_Player.m_TargetLookOffset = Vect3f(0.0, 0.0, 0.0)
end

function ClearPlayerCamera(_Player)
utils_log("ClearPlayerCamera")
	if _Player.m_CameraAnimation ~= nil then
		l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
		l_CameraControllerManager:get_resource(_Player.m_CameraControllerName):copy_from_key_camera(l_CameraControllerManager:get_main_camera():get_camera_as_info())
		l_CameraControllerManager:choose_main_camera(_Player.m_CameraControllerName)
	end
	_Player.m_CameraAnimation = nil
	utils_log("ClearPlayerCamera2")
end

function ClearPlayerCinematic(_Player)
utils_log("ClearPlayerCinematic")
	if _Player.m_InteractingCinematic ~= nil then
		_Player.m_CinematicManager:get_resource(_Player.m_InteractingCinematic):stop()
	end
	_Player.m_InteractingCinematic = nil
	utils_log("ClearPlayerCinematic2")
end

function ClearPlayerAend(_Player)
utils_log("ClearPlayerAend")
	if _Player.m_CurrentAend ~= nil then
		_Player.m_PhysXManager:character_controller_warp("player", _Player.m_Aends[_Player.m_CurrentAend])
		local l_NewControllerPosition = _Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
		l_Owner:set_position(l_NewControllerPosition)
	end
	_Player.m_CurrentAend = nil
	utils_log("ClearPlayerAend2")
end

function ClearPlayerStates(_Player)
utils_log("ClearPlayerStates")
	_Player.m_IsInteracting = false
	_Player.m_IsPuzzle = false
	_Player.m_IsClimbing = false
	_Player.m_IsCorrecting = false
	utils_log("ClearPlayerStates2")
end