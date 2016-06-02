--dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonEnemy.lua")


class 'CEnemy' (CLUAComponent)
	function CEnemy:__init(_TreeNode)
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)		
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self.m_PathFindig = CAStar()
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_WalkSpeed = 0.5
		self.m_RunSpeed = 2.5
		self.m_Patrol = _TreeNode:get_psz_property("patrol", "", false)
		
		self.m_distance_to_activate = 10.0
		self.m_off = true
		self.m_time_to_teleport = 1.0
		self.m_teleport_distance = 5.0
		self.m_teleport_timer = 0
		self.m_time_sin_mirar = 1.5
		self.m_timer_to_stop = 0
		self.m_alert_timer = 0
		self.m_distance_to_kill = 1.0
		
		self.m_StateMachine = StateMachine.create()
		self.m_PhysXManager:create_character_controller(self.m_Name, 1.2, 0.3, 0.5, self.m_RenderableObject:get_position(),"controllerMaterial", "Enemy")
	end
	
	function CEnemy:Update(_ElapsedTime)
		utils_log("CEnemy:Update")
	end
--end