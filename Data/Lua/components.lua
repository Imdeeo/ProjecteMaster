function InitComponents()
	InitBoxMove()
end

function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("porsche")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "", "")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
end

function FnOnCreateController ()
	local l_Box=CUABEngine.get_instance():get_layer_manager():get_resource("solid"):get_resource("porsche")

	--create_character_controller("player", 1.5, 1, 0.5, l_Box:get_pos(),"", 1)
end

function FnOnDestroyController ()
	
end

function FnOnUpdateController ()

end