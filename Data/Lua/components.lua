function InitComponents()
	InitBoxMove()
end

function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "","", "", "", "")
		--local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateWitcher","FnOnDestroyWitcher", "FnOnUpdateWitcher", "", "")
		--l_Box:AddComponent(l_Component)
		if l_Component == nil then
			utils_log("es_nulo");
		else
			utils_log("es_ok");
		end
	end
end

