dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonEnemy.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogEnemy.lua")

class 'CEnemy'
	function CEnemy:__init()
	end

	function CEnemy:InitEnemy(_TreeNode)
		utils_log("Init enemy")

		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self.m_PathFindig = CAStar()
		self.m_Type = _TreeNode:get_psz_property("type", "", false)
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_Speed = 1.0
		self.m_Patrol = false
		
		self.m_distance_to_activate = 10.0
		self.m_off = true
		self.m_time_to_teleport = 1.0
		self.m_teleport_distance = 5.0
		self.m_teleport_timer = 0
		self.m_distance_to_kill = 1.0
		
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent"..self.m_Type)
				
		if l_Component==nil then
			l_Component=create_scripted_component("ScriptedComponent"..self.m_Type, self.m_RenderableObject, "FnOnCreate"..self.m_Type,"FnOnDestroy"..self.m_Type, "FnOnUpdate"..self.m_Type, "FnOnRender"..self.m_Type, "FnOnDebugRender"..self.m_Type)
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent"..self.m_Type, l_Component)
		end
		
		if self.m_Type == "Automaton" then
			setAutomatonStateMachine()
			AutomatonStateMachine:start()
		elseif self.m_Type == "FogAutomaton" then
			setFogAutomatonStateMachine()
			FogAutomatonStateMachine:start()
		elseif self.m_Type == "Turret" then
			setTurretEnemyStateMachine()
			TurretEnemyStateMachine:start()
		elseif self.m_Type == "Whisperer" then
			setWhispererEnemyStateMachine()
			WhispererEnemyStateMachine:start()
		end
	end
--end