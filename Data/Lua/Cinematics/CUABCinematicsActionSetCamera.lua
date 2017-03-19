class 'CUABCinematicsActionSetCamera' (CUABCinematicsAction)
	function CUABCinematicsActionSetCamera:__init(TreeNode)
		CUABCinematicsAction.__init(self, TreeNode)
		self.m_CameraName=TreeNode:get_psz_property("name", "")
	end
	
	function CUABCinematicsActionSetCamera:Execute()
		local l_Engine = CUABEngine.get_instance()
		local l_CameraManager=l_Engine:get_camera_controller_manager()
		utils_log(""..self.m_CameraName)
		l_CameraManager:choose_main_camera(self.m_CameraName)
	end
--end