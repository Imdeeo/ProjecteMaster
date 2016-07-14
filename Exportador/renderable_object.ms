fileIn "math.ms"
fileIn "utils.ms"

fn createInstanceMesh dir filename Objs=
(	
	local pos
	local rot
	local core
	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	format "\t<layer name=\"solid\" default=\"true\"/>\n" to:file
	format "\t<layer name=\"alpha_objects\"/>\n" to:file
	format "\t<layer name=\"alpha_blend_objects\"/>\n" to:file
	format "\t<layer name=\"particles\"/>\n" to:file
	format "\t<layer name=\"triggers\"/>\n" to:file
	format "\t<layer name=\"manchas\"/>\n" to:file
	format "\t<layer name=\"skybox\"/>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			pos = Objs[i].transform.translation
			TranslationToRH &pos
			
			rot = Objs[i].rotation
			QuatToRH &rot
			
			if (getUserProp Objs[i] "render_layer") == "triggers" then
			(
				format "\t<instance_mesh name=\"%\" layer =\"triggers\" on_trigger_enter=\"%\" on_trigger_exit=\"%\" on_trigger_stay=\"%\" is_active=\"true\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"true\" physics_type=\"%\" physics_material=\"%\" physics_group=\"%\" visible=\"%\">\n" Objs[i].name (getUserProp Objs[i] "on_trigger_enter") (getUserProp Objs[i] "on_trigger_exit") (getUserProp Objs[i] "on_trigger_stay") core pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w (getUserProp Objs[i] "physics_generate_type") (getUserProp Objs[i] "physics_material") (getUserProp Objs[i] "physics_layer") true to:file
				format "\t\t<active_actor actor_name=\"player\"/>" to:file
				format "\t</instance_mesh>" to:file
			) else (
				if((getUserProp Objs[i] "render_export_type") == "animated_instance_mesh") then
				(
					core = (getUserProp Objs[i] "anim_model_core")
				) else (
					core = (getUserProp Objs[i] "core_name")
				)
				
				if (getUserProp Objs[i] "physics_generate") then
				(
					if ((getUserProp Objs[i] "physics_generate_type") == "plane_shape") then
					(
						format "\t<% name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"true\" physics_type=\"plane_shape\" physics_offset=\"%\" physics_normal=\"% % %\" physics_material=\"%\" physics_group=\"%\" visible=\"%\"/>\n" (getUserProp Objs[i] "render_export_type") Objs[i].name (getUserProp Objs[i] "render_layer") core pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w (getUserProp Objs[i] "plane_offset") (getUserProp Objs[i] "plane_normal_x") (getUserProp Objs[i] "plane_normal_y") (getUserProp Objs[i] "plane_normal_z") (getUserProp Objs[i] "physics_material") (getUserProp Objs[i] "physics_layer") true to:file
					) else (
						format "\t<% name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"true\" physics_type=\"%\" physics_material=\"%\" physics_group=\"%\" visible=\"%\"/>\n" (getUserProp Objs[i] "render_export_type") Objs[i].name (getUserProp Objs[i] "render_layer") core pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w (getUserProp Objs[i] "physics_generate_type") (getUserProp Objs[i] "physics_material") (getUserProp Objs[i] "physics_layer") true to:file
					)
				) else (
					format "\t<% name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"false\" visible=\"%\"/>\n" (getUserProp Objs[i] "render_export_type") Objs[i].name (getUserProp Objs[i] "render_layer") core pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w true to:file
				)
			)
		)
	)
	format "</renderable_objects>\n" to:file
	close file
)
