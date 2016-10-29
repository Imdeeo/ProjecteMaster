g_PI = 3.1415926535897932384626433832795

function GetTriggerPos(_Target, _LevelId)
	return CUABEngine.get_instance():get_level_manager():get_level(_LevelId):get_layer_manager():get_resource("triggers"):get_resource(_Target):get_position() - Vect3f(0.0, g_TotalHeight, 0.0)
end

function GetObjectPos(_Target, _LevelId)
	return CUABEngine.get_instance():get_level_manager():get_level(_LevelId):get_layer_manager():get_resource("solid"):get_resource(_Target):get_position()
end
