dofile("Data\\Lua\\Player\\PlayerStateMachine.lua")

class 'CPlayer'
	function CPlayer:__init()
		
	end

	function CPlayer:InitPlayer(_TreeNode)
		utils_log("Init player")
		--local UABEngine = CUABEngine.get_instance()
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_CameraControllerName= _TreeNode:get_psz_property("camera_controller", "", false)
		self.m_CameraController = UABEngine:get_camera_controller_manager():get_resource(self.m_CameraControllerName)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		self.m_InputManager = CInputManager.get_input_manager()
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_Speed = 5
		self.m_Sanity = 100
		self.m_MaxSanity = 100
		
		local l_Component = self.m_RenderableObject:get_component_manager():get_resource("ScriptedComponent")
		
		if l_Component==nil then
			local l_Component=create_scripted_component("ScriptedComponent", self.m_RenderableObject, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "FnOnDebugRender")
			self.m_RenderableObject:get_component_manager():add_resource("ScriptedComponent", l_Component)
		end
		
		setPlayerStateMachine()
		PlayerStateMachine:start()
	end
	
	function CPlayer:SetSanity(_amount, _override)
		self.m_Sanity = _amount
		if _override then
			self.m_MaxSanity = _amount
		end
	end
	
	function CPlayer:ModifySanity(_amount, _override)
		self.m_Sanity = self.m_Sanity + _amount
		if _override and self.m_Sanity > self.m_MaxSanity then
			self.m_MaxSanity = self.m_Sanity
		end
	end
	
	function CPlayer:RecoverSanity()
		self.m_Sanity = self.m_MaxSanity
	end
	
	function CPlayer:GainSanity()
		self.m_Sanity = self.m_Sanity + 10
		if self.m_Sanity > self.m_MaxSanity then
			self.m_Sanity = self.m_MaxSanity
		end
	end
	
	function CPlayer:LoseSanity()
		self.m_Sanity = self.m_Sanity - 10
		if self.m_Sanity < 0 then
			self.m_Sanity = 0
		end
	end

--end

function FnOnCreateController (_owner)
	g_Player.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
	g_Player.m_PhysXManager:create_character_controller(g_Player.m_Name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", 1)
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