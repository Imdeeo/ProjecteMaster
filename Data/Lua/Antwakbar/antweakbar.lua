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
	DebugHelper:add_lua_button("Render Commands","RegisterSceneRendererCommandsBar()","");
	DebugHelper:add_lua_button("Cameras","RegisterCamerasBar()","");
	DebugHelper:add_lua_button("Lights","RegisterLightsBar()","");
	DebugHelper:add_lua_button("Particles","RegisterParticlesBar()","");
	DebugHelper:add_lua_button("Manchas","RegisterManchasBar()","");
	
	DebugHelper:register_bar()
	
end

function RegisterShadersBar()
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Shaders")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Shaders\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_effect_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Shaders\");RegisterShadersBar()","");
	
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
	EffectTechnique:get_geometry_shader():reload()
end

function RegisterParametersVariables(material_name)
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()

	local bar_name = "Material: "..material_name
	
	DebugHelper:remove_bar("Materials")
	DebugHelper:start_register_bar(bar_name)
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\""..bar_name.."\");RegisterMaterialsBar()","");
	
	ShowMaterialParameters(DebugHelper, UABEngine, material_name)
	
	DebugHelper:register_bar()
	
end

function ShowMaterialParameters(DebugHelper, UABEngine, material_name)
	local MaterialsManager = UABEngine:get_material_manager()
	local Material = MaterialsManager:get_resource(material_name)
	local MaterialParameters = Material:get_parameters()
	
	for parameter in MaterialParameters do
		if(parameter:get_material_type()==CMaterialParameter.float)then
		
			DebugHelper:add_variable(parameter.name,CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(0),parameter:get_description())--" group='"..material_name.."' ")

		elseif(parameter:get_material_type()==CMaterialParameter.vect2f)then

			DebugHelper:add_variable(parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(0),parameter:get_description())--"" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(1),parameter:get_description())--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
			
		elseif(parameter:get_material_type()==CMaterialParameter.vect3f)then
		
			DebugHelper:add_variable(parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(0),parameter:get_description())--" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(1),parameter:get_description())--" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": z",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(2),parameter:get_description())--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
			
		elseif(parameter:get_material_type()==CMaterialParameter.vect4f)then
		
			DebugHelper:add_variable(parameter.name..": x",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(0),parameter:get_description())--" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": y",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(1),parameter:get_description())--" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": z",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(2),parameter:get_description())--" group="..parameter.name.." ")
			DebugHelper:add_variable(parameter.name..": w",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(3),parameter:get_description())--" group="..parameter.name.." \n Materials/"..parameter.name.." group="..material_name.." ")
			
		elseif(parameter:get_material_type()==CMaterialParameter.color)then
			
			DebugHelper:add_variable(parameter.name,CDebugHelper.color,CDebugHelper.read_write,parameter:get_value_address(0),parameter:get_description())--" group='"..material_name.."' ")
			
		end
	end
end

function RegisterMaterialsBar()
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Materials")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Materials\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_material_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Materials\");RegisterMaterialsBar()","");
	DebugHelper:add_lua_button("Save","CUABEngine.get_instance():get_material_manager():save()","");
	
	local MaterialsManager = UABEngine:get_material_manager()
	local Materials = MaterialsManager:get_elements_array()
	for i = 0,MaterialsManager:size()-1 do
		DebugHelper:add_lua_button(Materials[i].name,"RegisterParametersVariables(\""..Materials[i].name.."\")","");
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
	DebugHelper:add_lua_button("Layer Manager","CUABEngine.get_instance():get_layer_manager():reload();","");
	DebugHelper:add_lua_button("Camera Controllers","CUABEngine.get_instance():get_camera_controller_manager():reload();","");
	DebugHelper:add_lua_button("Light Controllers","CUABEngine.get_instance():get_light_manager():reload();","");
	
	DebugHelper:register_bar()
	
end

function RegisterSceneRendererCommandsBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Scene Renderer Commands")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Scene Renderer Commands\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_scene_command_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Scene Renderer Commands\");RegisterSceneRendererCommandsBar()","");
	
	local SceneRendererCommandsManager = UABEngine:get_scene_command_manager()
	local RendererCommands = SceneRendererCommandsManager:get_vector() --get_address
	
	for command in RendererCommands do
		
		DebugHelper:add_variable(command.name,CDebugHelper.bool,CDebugHelper.read_write,command:get_address(),"")--" group='"..material_name.."' ")
		
	end
	
	DebugHelper:register_bar()
	
end

function RegisterCamerasBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Cameras")
	
	local CameraControllerManager = UABEngine:get_camera_controller_manager()
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Cameras\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_camera_controller_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Cameras\");RegisterCamerasBar()","");
	
	local Cameras = CameraControllerManager:get_elements_array() --get_address
	local DebugCamera = CameraControllerManager:get_debug_camera()
	local MainCamera = CameraControllerManager:get_main_camera()
	
	DebugHelper:add_variable("Debug Camera: ",CDebugHelper.string,CDebugHelper.read,DebugCamera:get_name_address(),"")
	DebugHelper:add_variable("Main Camera: ",CDebugHelper.string,CDebugHelper.read,MainCamera:get_name_address(),"")
	DebugHelper:add_lua_button("-------------------------------------","","")
	for i = 0,CameraControllerManager:size()-1 do
		
		local camera = Cameras[i]
		DebugHelper:add_lua_button("Set "..camera.name.." Debug Camera","CUABEngine.get_instance():get_camera_controller_manager():choose_debug_camera(\""..camera.name.."\")","")
		DebugHelper:add_lua_button("Set "..camera.name.." Main Camera","CUABEngine.get_instance():get_camera_controller_manager():choose_main_camera(\""..camera.name.."\")","")
		
	end
	
	DebugHelper:register_bar()
	
end

function RegisterLightsBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Lights")
	
	local LightControllerManager = UABEngine:get_light_manager()
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Lights\");RegisterMainBar()","")
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_light_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Lights\");RegisterLightsBar()","")
	DebugHelper:add_lua_button("Save","CUABEngine.get_instance():get_light_manager():save()","")
	DebugHelper:add_lua_button("--------------------------------------","","")
	DebugHelper:add_lua_button("Add New Omni","CreateNewLight(1)","")
	DebugHelper:add_lua_button("Add New Spot","CreateNewLight(2)","")
	DebugHelper:add_lua_button("Add New Directional","CreateNewLight(3)","")	
	DebugHelper:add_lua_button("---------------------------------------","","")
	local Lights = LightControllerManager:get_elements_array() --get_address
	
	for i = 0,LightControllerManager:size()-1 do
		DebugHelper:add_lua_button(Lights[i].name,"RegisterLightParameters(\""..Lights[i].name.."\")","")
	end
	
	DebugHelper:register_bar()
	
end

function RegisterLightParameters(light_name)
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	local LightsManager = UABEngine:get_light_manager()
	
	local bar_name = "Light: "..light_name
	
	DebugHelper:remove_bar("Lights")
	DebugHelper:start_register_bar(bar_name)
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\""..bar_name.."\");RegisterLightsBar()","");
	
	local Light = LightsManager:get_resource(light_name)

	DebugHelper:add_variable("Enable",CDebugHelper.bool,CDebugHelper.read_write,Light:get_enable_lua_address(),"")
	DebugHelper:add_variable("Intensity",CDebugHelper.float,CDebugHelper.read_write,Light:get_intensity_lua_address(),"min=0.0 max=1.0 step=0.1")
	DebugHelper:add_variable("Shadow Map",CDebugHelper.bool,CDebugHelper.read_write,Light:get_generate_shadowmap_lua_address(),"")
	DebugHelper:add_variable("Color",CDebugHelper.color,CDebugHelper.read_write,Light:get_color_lua_address(),"")
	DebugHelper:add_variable("Start",CDebugHelper.float,CDebugHelper.read_write,Light:get_start_range_attenuation_lua_address(),"min=0.0 max=500.0 step=0.1 group=\"Range Attenuation\"")
	DebugHelper:add_variable("End",CDebugHelper.float,CDebugHelper.read_write,Light:get_end_range_attenuation_lua_address(),"min=0.0 max=500.0 step=0.1 group=\"Range Attenuation\"")
	DebugHelper:add_variable("Pos X:",CDebugHelper.float,CDebugHelper.read_write,Light:get_position_lua_address(0),"min=-250.0 max=250.0 step=0.1 group=\"Position\"")
	DebugHelper:add_variable("Pos Y:",CDebugHelper.float,CDebugHelper.read_write,Light:get_position_lua_address(1),"min=-250.0 max=250.0 step=0.1 group=\"Position\"")
	DebugHelper:add_variable("Pos Z:",CDebugHelper.float,CDebugHelper.read_write,Light:get_position_lua_address(2),"min=-250.0 max=250.0 step=0.1 group=\"Position\"")

	if(Light:get_type()==1)then
		-- DIRECTIONAL
		local Directional = LightsManager:get_resource(light_name)
		DebugHelper:add_variable("X:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(0),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
		DebugHelper:add_variable("Y:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(1),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
		DebugHelper:add_variable("Z:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(2),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
	elseif(Light:get_type()==2)then
		-- SPOT
		local Spot = LightsManager:get_resource(light_name)
		DebugHelper:add_variable("X:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(0),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
		DebugHelper:add_variable("Y:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(1),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
		DebugHelper:add_variable("Z:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(2),"min=-50.0 max=50.0 step=0.1 group=\"Direction\"")
		DebugHelper:add_variable("Angle:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_angle_lua_address(),"min=-50.0 max=50.0 step=0.1")
		DebugHelper:add_variable("FallOff:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_falloff_lua_address(),"min=-50.0 max=50.0 step=0.1")
	end
	
	DebugHelper:register_bar()
	
end

function CreateNewLight(_type)
	local UABEngine = CUABEngine.get_instance()	
	local LightsManager = UABEngine:get_light_manager()
	local Name = ""
	
	if _type == 1 then
		Name = "Omni"..LightsManager:size()
		LightsManager:create_new_light(Name,"omni")
	elseif _type == 2 then
		Name = "Spot"..LightsManager:size()
		LightsManager:create_new_light(Name,"spot")
	else
		Name = "Directional"..LightsManager:size()
		LightsManager:create_new_light(Name,"directional")
	end
	
	--RegisterLightParameters(LightsManager:get_resource(Name))
	RegisterLightsBar()

end

function RegisterParticlesBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Particles")
	
	local LayerParticleControllerManager = UABEngine:get_layer_manager()
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Particles\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_particle_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Particles\");RegisterParticlesBar()","");
	DebugHelper:add_lua_button("Save","SaveParticles()","");
	
	local ParticlesLayer = LayerParticleControllerManager:get_layer("particles") --get_address
	
	local Particles = ParticlesLayer:get_elements_array()
	
	for i = 0,ParticlesLayer:size()-1 do
		DebugHelper:add_lua_button(Particles[i].name,"RegisterParticleParameters(\""..Particles[i].name.."\")","");
	end
	
	DebugHelper:register_bar()
	
end

function RegisterParticleParameters(particle_name)
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	local LayerParticlesManager = UABEngine:get_layer_manager()
	
	local bar_name = "Particle: "..particle_name
	
	DebugHelper:remove_bar("Particles")
	DebugHelper:start_register_bar(bar_name)
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\""..bar_name.."\");RegisterParticlesBar()","");
	
	local Particle = LayerParticlesManager:get_layer("particles"):get_resource(particle_name)
	local ParticleType = Particle:get_type()
	local Material = ParticleType:get_material()
	local Texture = Material:get_texture(0)
	
	DebugHelper:add_variable("Texture Name",CDebugHelper.string,CDebugHelper.read_write,Texture:get_name_address(),"")
	DebugHelper:add_lua_button("Reload Texture","ReloadParticle(\""..Texture.name:gsub("\\","+").."\")","")
	DebugHelper:add_variable("Loop Frames",CDebugHelper.bool,CDebugHelper.read_write,ParticleType:get_lua_loop_frames(),"")
	DebugHelper:add_variable("Emit Absolute Max",CDebugHelper.bool,CDebugHelper.read_write,ParticleType:get_emit_absolute(),"")
	DebugHelper:add_variable("Num Frames",CDebugHelper.int,CDebugHelper.read_write,ParticleType:get_lua_num_frames(),"min=0.0 max=100.0 step=1.0")
	DebugHelper:add_variable("Start Acc Angle",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration_angle(),"min=0.0 max=100.0 step=0.1")
	DebugHelper:add_variable("Start Dir Angle",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_direction_angle(),"min=0.0 max=100.0 step=0.1")
	DebugHelper:add_variable("Time Frame",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_time_per_frame(),"min=0.0 max=100.0 step=0.1")	
	
	ShowMaterialParameters(DebugHelper, UABEngine, Material.name)
	
	DebugHelper:add_variable("Pos X",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(0),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Pos Y",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(1),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Pos Z",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(2),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Size X",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(0),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Size Y",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(1),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Size Z",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(2),"min=-100.0 max=100.0 step=0.01 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Angular Acc Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_angular_acceleration(0),"min=0.0 max=100.0 step=0.1 group=\"Angular Acceleration\"")
	DebugHelper:add_variable("Angular Acc Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_angular_acceleration(1),"min=0.0 max=100.0 step=0.1 group=\"Angular Acceleration\"")
	DebugHelper:add_variable("Awake Time Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_awake_time(0),"min=0.0 max=100.0 step=0.1 group=\"Awake Time\"")
	DebugHelper:add_variable("Awake Time Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_awake_time(1),"min=0.0 max=100.0 step=0.1 group=\"Awake Time\"")
	DebugHelper:add_variable("Emit Rate Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_emit_rate(0),"min=0.0 max=100.0 step=0.1 group=\"Emit Rate\"")
	DebugHelper:add_variable("Emit Rate Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_emit_rate(1),"min=0.0 max=100.0 step=0.1 group=\"Emit Rate\"")
	DebugHelper:add_variable("Sleep Time Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_sleep_time(0),"min=0.0 max=100.0 step=0.1 group=\"Sleep Time\"")
	DebugHelper:add_variable("Sleep Time Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_sleep_time(1),"min=0.0 max=100.0 step=0.1 group=\"Sleep Time\"")
	DebugHelper:add_variable("Start Angle Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angle(0),"min=0.0 max=100.0 step=0.1 group=\"Start Angle\"")
	DebugHelper:add_variable("Start Angle Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angle(1),"min=0.0 max=100.0 step=0.1 group=\"Start Angle\"")
	DebugHelper:add_variable("Start Angular Speed Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angular_speed(0),"min=0.0 max=100.0 step=0.1 group=\"Start Angular Speed\"")
	DebugHelper:add_variable("Start Angular Speed Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angular_speed(1),"min=0.0 max=100.0 step=0.1 group=\"Start Angular Speed\"")
	DebugHelper:add_variable("Life Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_life(0),"min=0.0 max=250.0 step=0.1 group=\"Life\"")
	DebugHelper:add_variable("Life Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_life(1),"min=0.0 max=250.0 step=0.1 group=\"Life\"")

	local index = ParticleType:get_sizes_control_points_size()
	for i = 0,index-1 do
		DebugHelper:add_variable("Size Width"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_size(i,0),"min=0.0 max=100.0 step=0.01 group=\"Control Point Size "..(i+1).."\"")
		DebugHelper:add_variable("Size Height"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_size(i,1),"min=0.0 max=100.0 step=0.01 group=\"Control Point Size "..(i+1).."\"")
		DebugHelper:add_variable("STime Min"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_size_time(i,0),"min=0.0 max=100.0 step=0.1 group=\"Control Point Size "..(i+1).."\"")
		DebugHelper:add_variable("STime Max"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_size_time(i,1),"min=0.0 max=100.0 step=0.1 group=\"Control Point Size "..(i+1).."\"")
	end
	
	local index2 = ParticleType:get_colors_control_points_size()	
	for i = 0,index2-1 do
		DebugHelper:add_variable("First Color"..(i+1),CDebugHelper.color,CDebugHelper.read_write,ParticleType:get_lua_control_point_color1(i),"group=\"Control Point Color "..(i+1).."\"")
		DebugHelper:add_variable("Second Color"..(i+1),CDebugHelper.color,CDebugHelper.read_write,ParticleType:get_lua_control_point_color2(i),"group=\"Control Point Color "..(i+1).."\"")
		DebugHelper:add_variable("CTime Min"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_color_time(i,0),"min=0.0 max=100.0 step=0.1 group=\"Control Point Color "..(i+1).."\"")
		DebugHelper:add_variable("CTime Max"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_color_time(i,1),"min=0.0 max=100.0 step=0.1 group=\"Control Point Color "..(i+1).."\"")
	end
	
	local index3 = ParticleType:get_speeds_control_points_size()
	for i = 0,index3-1 do
		DebugHelper:add_variable("First X Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed1(i,0),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("First Y Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed1(i,1),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("First Z Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed1(i,2),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("Second X Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed2(i,0),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("Second Y Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed2(i,1),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("Second Z Speed"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed2(i,2),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("SPTime Min"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed_time(i,0),"min=0.0 max=100.0 step=0.1 group=\"Control Point Speed "..(i+1).."\"")
		DebugHelper:add_variable("SPime Max"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_speed_time(i,1),"min=0.0 max=100.0 step=0.1 group=\"Control Point Speed "..(i+1).."\"")
	end
	
	local index4 = ParticleType:get_accelerations_control_points_size()
	for i = 0,index4-1 do
		DebugHelper:add_variable("First X Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration1(i,0),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("First Y Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration1(i,1),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("First Z Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration1(i,2),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("Second X Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration2(i,0),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("Second Y Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration2(i,1),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("Second Z Acc"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration2(i,2),"min=-100.0 max=100.0 step=0.01 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("ACCTime Min"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration_time(i,0),"min=0.0 max=100.0 step=0.1 group=\"Control Point Acc "..(i+1).."\"")
		DebugHelper:add_variable("ACCTime Max"..(i+1),CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_control_point_acceleration_time(i,1),"min=0.0 max=100.0 step=0.1 group=\"Control Point Acc "..(i+1).."\"")
	end
	
	DebugHelper:register_bar()
end

function SaveParticles()
	CUABEngine.get_instance():get_particle_manager():save()
	CUABEngine.get_instance():get_material_manager():save()
	CUABEngine.get_instance():get_layer_manager():save()
end 

function ReloadParticle(_textureName)
	local stringa = _textureName:gsub("+","\\")
	local cono = CUABEngine.get_instance():get_texture_manager():get_resource(stringa):reload()
end

function RegisterManchasBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Manchas")
	
	local LayerManchasControllerManager = UABEngine:get_layer_manager()
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Manchas\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_manchas_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Manchas\");RegisterManchasBar()","");
	--DebugHelper:add_lua_button("Save","SaveManchas()","");
	
	local ManchasLayer = LayerManchasControllerManager:get_layer("manchas") --get_address
	
	local Manchas = ManchasLayer:get_elements_array()
	
	for i = 0,ManchasLayer:size()-1 do
		DebugHelper:add_lua_button(Manchas[i].name,"RegisterManchasParameters(\""..Manchas[i].name.."\")","");
	end
	
	DebugHelper:register_bar()
end

function RegisterManchasParameters(manchas_name)
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	local LayerManchasControllerManager = UABEngine:get_layer_manager()
	
	local bar_name = "Mancha: "..manchas_name
	
	DebugHelper:remove_bar("Manchas")
	DebugHelper:start_register_bar(bar_name)
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\""..bar_name.."\");RegisterManchasBar()","");
	
	local Manchas = LayerManchasControllerManager:get_layer("manchas"):get_resource(manchas_name)
	local ManchasType = Manchas:get_type()
	--local Material = ManchasType:get_material()
	--local Texture = Material:get_texture(0)
			
	DebugHelper:add_variable("Frames",CDebugHelper.int,CDebugHelper.read_write,ManchasType:get_lua_frames(),"min=1.0 max=100.0 step=1.0")
	DebugHelper:add_variable("Emit Time Min",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_emit_time(0),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Emit Time Max",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_emit_time(1),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Life Time Min",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_life_time(0),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Life Time Max",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_life_time(1),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Opacity Max 1",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_opacity(0),"min=0.0 max=10.0 step=0.05")
	DebugHelper:add_variable("Opacity Max 2",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_opacity(1),"min=0.0 max=10.0 step=0.05")
	DebugHelper:add_variable("Size Min",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_size(0),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Size Max",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_size(1),"min=0.1 max=100.0 step=0.1")
	DebugHelper:add_variable("Size Speed Min",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_size_speed(0),"min=0.0 max=10.0 step=0.1")
	DebugHelper:add_variable("Size Speed Max",CDebugHelper.float,CDebugHelper.read_write,ManchasType:get_lua_size_speed(1),"min=0.0 max=10.0 step=0.1")
	DebugHelper:add_variable("Color 1",CDebugHelper.color,CDebugHelper.read_write,ManchasType:get_lua_color1(),"")
	DebugHelper:add_variable("Color 2",CDebugHelper.color,CDebugHelper.read_write,ManchasType:get_lua_color2(),"")

	DebugHelper:register_bar()
end