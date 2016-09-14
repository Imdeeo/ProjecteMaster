class 'CUABCinematicsAction'
	function CUABCinematicsAction:__init(TreeNode)
		self.m_Time=TreeNode:get_float_property("time", 0.0)
	end
	
	function CUABCinematicsAction:GetTime()
		return self.m_Time
	end
	
	function CUABCinematicsAction:SetTime(Value)
		self.m_Time=Value
	end
	
	function CUABCinematicsAction:GetDuration()
		return 0
	end
	
	function CUABCinematicsAction:IsActive()
		return false
	end
	
	function CUABCinematicsAction:Update(ElapsedTime)
	end
	
	function CUABCinematicsAction:MustUpdate()
		return false
	end
	
	function CUABCinematicsAction:Execute()
	end
--end