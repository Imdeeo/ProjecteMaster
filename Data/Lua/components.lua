function InitComponents()
	InitBoxMove()
end

function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "", "")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
end

function FnOnCreateController ()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	local l_physXManager = UABEngine:get_physX_manager()
	l_physXManager:register_material("controllerMaterial", 0.5,0.5,1)
	l_physXManager:create_character_controller("player", 1.5, 1, 0.5, l_Box:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
	-- destruir character controller
end

function FnOnUpdateController ()

end