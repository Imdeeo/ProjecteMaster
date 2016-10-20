	class 'CLuz' (CLUAComponent)
	function CLuz:__init(_TreeNode, _LevelId)
		local UABEngine = CUABEngine.get_instance()
		local l_Level = CUABEngine.get_instance():get_level_manager():get_level(_LevelId)
		self.m_Name = _TreeNode:get_psz_property("name", "")
		utils_log("name: "..self.m_Name)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "")
		utils_log("layer: "..self.m_LayerName)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "")
		utils_log("renderable_object: "..self.m_RenderableObjectName)
		self.m_RenderableObject = l_Level:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)
		self.m_Luz = l_Level:get_light_manager():get_resource(self.m_RenderableObjectName)
		self.m_Luz:set_intensity(3)
		self.m_Timer = 0.0
	end
	
	function CLuz:Update(_ElapsedTime)
		self.m_Timer = self.m_Timer + _ElapsedTime
		--if self.m_Timer >= 0.08 then 
			self.m_Timer = 0
			self.m_Luz:set_intensity(2.5+math.random())
		--end
		--[[if not self.m_Swap then
			local l_swap = math.random()
			utils_log(""..l_swap)
			if l_swap > 0.2 then
				self.m_Swap = true
				local l_swap2 = math.random()
				if l_swap2 > 0.5 and (self.m_Luz:get_intensity()+l_swap) < 4.8 then 
					self.m_Luz:set_intensity(self.m_Luz:get_intensity()+l_swap)
				else
					if (self.m_Luz:get_intensity()-l_swap) < 0.2 then 
						self.m_Luz:set_intensity(self.m_Luz:get_intensity()-l_swap)
					end
				end
			end		
		else
			self.m_Swap = false
		end]]
		
		--self.m_Luz:set_intensity(math.random())
		--self.m_Luz:set_intensity(2)
		
	end