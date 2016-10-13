	class 'CLuz' (CLUAComponent)
	function CLuz:__init(_TreeNode)
		local UABEngine = CUABEngine.get_instance()
		self.m_Intensidades ={}
		self.m_Name = _TreeNode:get_psz_property("name", "")
		utils_log("name: "..self.m_Name)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "")
		utils_log("layer: "..self.m_LayerName)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "")
		utils_log("layer: "..self.m_RenderableObjectName)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)
		self.m_Luz = UABEngine:get_light_manager():get_resource(self.m_Name)	
		self.m_NumIntensidades = 0
		l_Element = _TreeNode:first_child()
		while l_Element ~= nil do
			table.insert(self.m_Intensidades, l_Element:get_float_property("value", 0.0))
			self.m_NumIntensidades = self.m_NumIntensidades+1 
			l_Element = l_Element:get_next()
		end
	end
	
	function CLuz:Update(_ElapsedTime)
		--utils_log("CLuz:Update")
	end