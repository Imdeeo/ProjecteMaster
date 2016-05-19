dofile("Data\\Lua\\Enemies\\Automaton\\AutomatonEnemy.lua")

class 'CEnemy'
	function CEnemy:__init()
		
	end

	function CEnemy:InitEnemy(_TreeNode)
		utils_log("Init enemy")
		--local UABEngine = CUABEngine.get_instance()
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self.m_Type = _TreeNode:get_psz_property("type", "", false)
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_Speed = 5
		
		if self.m_Type == "automaton" then
			InitAutomaton()
		if self.m_Type == "fog_automaton" then
			InitFogAutomaton()
		elseif self.m_Type == "turret" then
			IniteTurret()
		elseif self.m_Type == "whisperer"
			InitWhisperer()
		end
	end

	function CEnemy:InitAutomaton()
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
				
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", self.m_RenderableObject, "FnOnCreateAutomaton","FnOnDestroyAutomaton", "FnOnUpdateAutomaton", "FnOnRenderAutomaton", "FnOnDebugRenderAutomaton")
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end

		setAutomatonEnemyStateMachine()
		AutomatonEnemyStateMachine:start()
	end

	function CEnemy:InitFogAutomaton()
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
				
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", self.m_RenderableObject, "FnOnCreateFogAutomaton","FnOnDestroyFogAutomaton", "FnOnUpdateFogAutomaton", "FnOnRenderFogAutomaton", "FnOnDebugRenderFogAutomaton")
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end

		setAutomatonEnemyStateMachine()
		AutomatonEnemyStateMachine:start()
	end

	function CEnemy:InitTurret()
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
				
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", self.m_RenderableObject, "FnOnCreateTurret","FnOnDestroyTurret", "FnOnUpdateTurret", "FnOnRenderTurret", "FnOnDebugRenderTurret")
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end

		setAutomatonEnemyStateMachine()
		AutomatonEnemyStateMachine:start()
	end

	function CEnemy:InitWhisperer()
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
				
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", self.m_RenderableObject, "FnOnCreateWhisperer","FnOnDestroyWhisperer", "FnOnUpdateWhisperer", "FnOnRenderWhisperer", "FnOnDebugRenderWhisperer")
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end

		setAutomatonEnemyStateMachine()
		AutomatonEnemyStateMachine:start()
	end
