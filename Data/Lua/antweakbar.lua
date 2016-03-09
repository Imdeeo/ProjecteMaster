dofile("Data\\Lua\\Player\\PlayerATB.lua")
function InitAntweakBar()

	RegisterMainBar()

end

function RegisterMainBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:start_register_bar("MainBar")
	DebugHelper:add_lua_button("Shaders","RegisterShadersBar()","");
	DebugHelper:add_lua_button("Materials","RegisterMaterialsBar()","");
	DebugHelper:add_lua_button("Reload","RegisterReloadBar()","");
	DebugHelper:add_lua_button("Player","RegisterPlayerBar()","");
	DebugHelper:register_bar()
end


function RegisterShadersBar()
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Shaders")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Shaders\");RegisterMainBar()","");
	
	local EffectManager = UABEngine:get_effect_manager()
	local EffecTechniques = EffectManager:get_elements_array()
	for i = 0,EffectManager:size()-1 do
		DebugHelper:add_lua_button(EffecTechniques[i].name,"ReloadEffectTechnique(\""..EffecTechniques[i].name.."\")","");
	end
	
	DebugHelper:register_bar()
	
end

function ReloadEffectTechnique(EffectTechniqueName)

	local UABEngine = CUABEngine.get_instance()
	local EffectManager = UABEngine:get_effect_manager()
	local EffectTechnique = EffectManager:get_resource(EffectTechniqueName)
	EffectTechnique:get_vertex_shader():reload()
	EffectTechnique:get_pixel_shader():reload()

end

function AddParameterVariable(parameter,material_name)

	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	if(parameter:get_material_type()==CMaterialParameter.float)then
	
		utils_log("float")
		local addres = parameter:get_value_address()
		DebugHelper:add_variable(material_name.." - "..parameter.name,CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group='"..material_name.."' ")

	elseif(parameter:get_material_type()==CMaterialParameter.vect2f)then

		utils_log("vect2f")
		DebugHelper:add_variable(material_name.." - "..parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--"" group="..parameter.name.." ")
		DebugHelper:add_variable(material_name.." - "..parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
		
	elseif(parameter:get_material_type()==CMaterialParameter.vect3f)then
	
		DebugHelper:add_variable(material_name.." - "..parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." ")
		DebugHelper:add_variable(material_name.." - "..parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." ")
		DebugHelper:add_variable(material_name.." - "..parameter.name..": z",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
		
	elseif(parameter:get_material_type()==CMaterialParameter.vect4f)then
	
		DebugHelper:add_variable(parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." ")
		DebugHelper:add_variable(parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." ")
		DebugHelper:add_variable(parameter.name..": z",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." ")
		DebugHelper:add_variable(parameter.name..": w",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
		
	end
	
end

function RegisterMaterialsBar()
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Materials")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Materials\");RegisterMainBar()","");
	
	local MaterialsManager = UABEngine:get_material_manager()
	local Materials = MaterialsManager:get_elements_array()
	for i = 0,MaterialsManager:size()-1 do
		DebugHelper:add_lua_button(Materials[i].name,"ReloadMaterial(\""..Materials[i].name.."\")","");
		local MaterialParameters = Materials[i]:get_parameters()
		for parameter in MaterialParameters do
			AddParameterVariable(parameter,Materials[i].name)
		end
	end
	DebugHelper:register_bar()
	
end

function ReloadMaterial(MaterialName)

	local UABEngine = CUABEngine.get_instance()
	local MaterialsManager = UABEngine:get_material_manager()
	local Material = MaterialsManager:get_resource(MaterialName)

end

function RegisterReloadBar()
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Reload")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Reload\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Material Manager","CUABEngine.get_instance():get_material_manager():reload()","");
	DebugHelper:add_lua_button("Light Manager","CUABEngine.get_instance():get_light_manager():reload()","");
	--DebugHelper:add_lua_button("Textures","ReloadMaterial","");
	DebugHelper:add_lua_button("SceneComamands","CUABEngine.get_instance():get_scene_command_manager():reload()","");
	DebugHelper:add_lua_button("Effect Manager","CUABEngine.get_instance():get_effect_manager():reload()","");
	--DebugHelper:add_lua_button("Layer Manager","CUABEngine.get_instance():get_layer_manager():reload();CUABEngine.get_instance():get_scene_command_manager():reload()","");
	DebugHelper:add_lua_button("Camera Controllers","CUABEngine.get_instance():get_camera_controller_manager():reload();","");
	
	DebugHelper:register_bar()
end