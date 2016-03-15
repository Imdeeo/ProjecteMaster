class 'CUABCinematicsActionSetCamera' (CUABCinematicsAction)
	function CUABCinematicsActionSetCamera:__init(XMLTreeNode)
		CUABCinematicsAction.__init(self, XMLTreeNode)
		self.m_CameraName=XMLTreeNode:get_psz_property("name", "", false)
	end
	
	function CUABCinematicsActionSetCamera:Execute()
		local l_Engine = CUABEngine.get_instance()
		local l_CameraManager=l_Engine:get_camera_controller_manager()
		utils_log(""..self.m_CameraName)
		l_CameraManager:choose_main_camera(self.m_CameraName)
	end
--end