class 'CUABCinematicsActionSetCamera' (CUABCinematicsAction)
	function CUABCinematicsActionSetCamera:__init(XMLTreeNode)
		CUABCinematicsAction.__init(self, XMLTreeNode)
		self.m_CameraName=XMLTreeNode:GetPszProperty("name", "")
	end
	
	function CUABCinematicsActionSetCamera:Execute()
		local l_Engine=KGEngine()
		local l_CameraManager=l_Engine:GetCameraManager()
		l_CameraManager:SetControlCameraController(l_CameraManager:GetResource(self.m_CameraName))
	end
--end