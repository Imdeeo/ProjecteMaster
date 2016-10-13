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