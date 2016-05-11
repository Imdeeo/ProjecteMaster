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

end

function RegisterParametersVariables(material_name)
	
	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	local MaterialsManager = UABEngine:get_material_manager()
	
	local bar_name = "Material: "..material_name
	
	DebugHelper:remove_bar("Materials")
	DebugHelper:start_register_bar(bar_name)
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\""..bar_name.."\");RegisterMaterialsBar()","");
	
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
			
		end
	end
	
	DebugHelper:register_bar()
	
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
	
	utils_log("cosis1")
	local CameraControllerManager = UABEngine:get_camera_controller_manager()
	
	utils_log("cosis2")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Cameras\");RegisterMainBar()","");
	utils_log("cosis3")
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_camera_controller_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Cameras\");RegisterCamerasBar()","");
	utils_log("cosis4")
	
	local Cameras = CameraControllerManager:get_elements_array() --get_address
	local DebugCamera = CameraControllerManager:get_debug_camera()
	local MainCamera = CameraControllerManager:get_main_camera()
	
	utils_log("cosis5")
	
	DebugHelper:add_variable("Debug Camera: ",CDebugHelper.string,CDebugHelper.read,DebugCamera:get_name_address(),"")
	utils_log("cosis6")
	DebugHelper:add_variable("Main Camera: ",CDebugHelper.string,CDebugHelper.read,MainCamera:get_name_address(),"")
	utils_log("cosis7")
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
	
	utils_log("lights1")
	local LightControllerManager = UABEngine:get_light_manager()
	
	utils_log("lights2")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Lights\");RegisterMainBar()","");
	utils_log("lights3")
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_light_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Lights\");RegisterLightsBar()","");
	utils_log("lights4")
	
	local Lights = LightControllerManager:get_elements_array() --get_address
	
	for i = 0,LightControllerManager:size()-1 do
		DebugHelper:add_lua_button(Lights[i].name,"RegisterLightParameters(\""..Lights[i].name.."\")","");
	end
	
	utils_log("lights5")
	
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
	DebugHelper:add_variable("Start Range Attenuation",CDebugHelper.float,CDebugHelper.read_write,Light:get_start_range_attenuation_lua_address(),"min=0.0 max=500.0 step=1.0")
	DebugHelper:add_variable("End Range Attenuation",CDebugHelper.float,CDebugHelper.read_write,Light:get_end_range_attenuation_lua_address(),"min=0.0 max=500.0 step=1.0")
	DebugHelper:add_variable("Color",CDebugHelper.color,CDebugHelper.read_write,Light:get_color_lua_address(),"")
	DebugHelper:add_variable("Shadow Map",CDebugHelper.bool,CDebugHelper.read_write,Light:get_generate_shadowmap_lua_address(),"")

	if(Light:get_type()==1)then
		-- DIRECTIONAL
		local Directional = LightsManager:get_resource(light_name)
		DebugHelper:add_variable("Direction x:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(0),"")
		DebugHelper:add_variable("Direction y:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(1),"")
		DebugHelper:add_variable("Direction z:",CDebugHelper.float,CDebugHelper.read_write,Directional:get_direction_lua_address(2),"")
	elseif(Light:get_type()==2)then
		-- SPOT
		local Spot = LightsManager:get_resource(light_name)
		DebugHelper:add_variable("Direction x:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(0),"")
		DebugHelper:add_variable("Direction y:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(1),"")
		DebugHelper:add_variable("Direction z:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_direction_lua_address(2),"")
		DebugHelper:add_variable("Angle:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_angle_lua_address(),"")
		DebugHelper:add_variable("FallOff:",CDebugHelper.float,CDebugHelper.read_write,Spot:get_falloff_lua_address(),"")
	end
	
	DebugHelper:register_bar()
	
end

function RegisterParticlesBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Particles")
	
	local LayerParticleControllerManager = UABEngine:get_layer_manager()
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Particles\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Reload All","CUABEngine.get_instance():get_particle_manager():reload();CDebugHelper.get_debug_helper():remove_bar(\"Particles\");RegisterParticlesBar()","");
	
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

	DebugHelper:add_variable("Texture Name",CDebugHelper.string,CDebugHelper.read_write,ParticleType:get_lua_texture_name(),"")	
	DebugHelper:add_variable("Loop Frames",CDebugHelper.bool,CDebugHelper.read_write,ParticleType:get_lua_loop_frames(),"")
	DebugHelper:add_variable("Emit Absolute Max",CDebugHelper.bool,CDebugHelper.read_write,ParticleType:get_emit_absolute(),"")
	DebugHelper:add_variable("Num Frames",CDebugHelper.int,CDebugHelper.read_write,ParticleType:get_lua_num_frames(),"min=0.0 max=100.0 step=1.0")
	DebugHelper:add_variable("Start Acc Angle",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration_angle(),"min=0.0 max=100.0 step=0.1")
	DebugHelper:add_variable("Start Dir Angle",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_direction_angle(),"min=0.0 max=100.0 step=0.1")
	DebugHelper:add_variable("Time Frame",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_time_per_frame(),"min=0.0 max=100.0 step=0.1")
	DebugHelper:add_variable("Pos X",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(0),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Pos Y",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(1),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Pos Z",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_position(2),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Position\"")
	DebugHelper:add_variable("Size X",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(0),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Size Y",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(1),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Size Z",CDebugHelper.float,CDebugHelper.read_write,Particle:get_lua_emission_box_half_size(2),"min=0.0 max=100.0 step=0.1 group=\"Emission Box Half Size\"")
	DebugHelper:add_variable("Angular Acc Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_angular_acceleration(0),"min=0.0 max=100.0 step=0.1 group=\"Angular Acceleration\"")
	DebugHelper:add_variable("Angular Acc Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_angular_acceleration(1),"min=0.0 max=100.0 step=0.1 group=\"Angular Acceleration\"")
	DebugHelper:add_variable("Awake Time Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_awake_time(0),"min=0.0 max=100.0 step=0.1 group=\"Awake Time\"")
	DebugHelper:add_variable("Awake Time Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_awake_time(1),"min=0.0 max=100.0 step=0.1 group=\"Awake Time\"")
	--DebugHelper:add_variable("Color 1",CDebugHelper.color,CDebugHelper.read_write,ParticleType:get_lua_color1(),"")
	--DebugHelper:add_variable("Color 2",CDebugHelper.color,CDebugHelper.read_write,ParticleType:get_lua_color2(),"")
	DebugHelper:add_variable("Emit Rate Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_emit_rate(0),"min=0.0 max=100.0 step=0.1 group=\"Emit Rate\"")
	DebugHelper:add_variable("Emit Rate Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_emit_rate(1),"min=0.0 max=100.0 step=0.1 group=\"Emit Rate\"")
	--DebugHelper:add_variable("Size Width",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_size(0),"min=0.0 max=100.0 step=1.0")
	--DebugHelper:add_variable("Size Height",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_size(1),"min=0.0 max=100.0 step=1.0")
	DebugHelper:add_variable("Sleep Time Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_sleep_time(0),"min=0.0 max=100.0 step=0.1 group=\"Sleep Time\"")
	DebugHelper:add_variable("Sleep Time Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_sleep_time(1),"min=0.0 max=100.0 step=0.1 group=\"Sleep Time\"")
	DebugHelper:add_variable("Start Acc1 x",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration1(0),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 1\"")
	DebugHelper:add_variable("Start Acc1 y",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration1(1),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 1\"")
	DebugHelper:add_variable("Start Acc1 z",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration1(2),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 1\"")
	DebugHelper:add_variable("Start Acc2 x",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration2(0),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 2\"")
	DebugHelper:add_variable("Start Acc2 y",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration2(1),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 2\"")
	DebugHelper:add_variable("Start Acc2 z",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_acceleration2(2),"min=0.0 max=100.0 step=0.1 group=\"Starting Acceleration 2\"")
	DebugHelper:add_variable("Start Angle Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angle(0),"min=0.0 max=100.0 step=0.1 group=\"Start Angle\"")
	DebugHelper:add_variable("Start Angle Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angle(1),"min=0.0 max=100.0 step=0.1 group=\"Start Angle\"")
	DebugHelper:add_variable("Start Angular Speed Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angular_speed(0),"min=0.0 max=100.0 step=0.1 group=\"Start Angular Speed\"")
	DebugHelper:add_variable("Start Angular Speed Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_angular_speed(1),"min=0.0 max=100.0 step=0.1 group=\"Start Angular Speed\"")
	DebugHelper:add_variable("Start Speed1 x",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed1(0),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 1\"")
	DebugHelper:add_variable("Start Speed1 y",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed1(1),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 1\"")
	DebugHelper:add_variable("Start Speed1 z",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed1(2),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 1\"")
	DebugHelper:add_variable("Start Speed2 x",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed2(0),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 2\"")
	DebugHelper:add_variable("Start Speed2 y",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed2(1),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 2\"")
	DebugHelper:add_variable("Start Speed2 z",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_starting_speed2(2),"min=0.0 max=100.0 step=0.1 group=\"Starting Speed 2\"")
	DebugHelper:add_variable("Life Min",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_life(0),"min=0.0 max=100.0 step=0.1 group=\"Life\"")
	DebugHelper:add_variable("Life Max",CDebugHelper.float,CDebugHelper.read_write,ParticleType:get_lua_life(1),"min=0.0 max=100.0 step=0.1 group=\"Life\"")
	
	--local ControlPoints = ParticleType:get_control_points_size()
	
	DebugHelper:register_bar()
end