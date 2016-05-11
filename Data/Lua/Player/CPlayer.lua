dofile("Data\\Lua\\Player\\PlayerStateMachine.lua")

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
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_Speed = 10
		self.m_IsJumping = false
		self.m_IsAscending = false
		self.m_InputManager = CInputManager.get_input_manager()
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self:InitPlayer()
	end

	function CPlayer:InitPlayer()
		utils_log("Init player")
		local UABEngine = CUABEngine.get_instance()
		
		local l_Component = g_Player.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
		
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", g_Player.m_RenderableObject, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "FnOnDebugRender")
			g_Player.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end
		
		setPlayerStateMachine()
		PlayerStateMachine:start()
	end

	
--end

function FnOnCreateController (_owner)
	local UABEngine = CUABEngine.get_instance()
	g_Player.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
	g_Player.m_PhysXManager:create_character_controller(g_Player.name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
		
end

function FnOnUpdateController (_owner, _ElapsedTime)
	local args = {}
	args["owner"] = _owner
	PlayerStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderController(_owner, _rm)

end

function FnOnDebugRender(_owner, _rm)
	--g_PhysXManager:render("player", _rm)
end