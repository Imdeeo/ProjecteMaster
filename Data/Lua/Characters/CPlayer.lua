class 'CPlayer'
	function CPlayer:__init(_TreeNode)
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_CameraControllerName= _TreeNode:get_psz_property("camera_controller", "", false)
		self.m_CameraController = UABEngine:get_camera_controller_manager():get_resource(self.m_CameraControllerName)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
	end

	function CPlayer:Execute_lua_command()
		loadstring(self.m_LuaCommand)()	
	end
--end