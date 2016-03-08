fileIn "math.ms"
fileIn "utils.ms"

fn createInstanceMesh dir filename Objs=
(	
	local pos
	local rot
	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	format "\t<layer name=\"solid\" default=\"true\"/>\n" to:file
	format "\t<layer name=\"alpha_objects\"/>\n" to:file
	format "\t<layer name=\"alpha_blend_objects\"/>\n" to:file
	format "\t<layer name=\"particles\"/>\n" to:file
	format "\t<layer name=\"skybox\"/>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			if Objs[i].material != undefined then
			(
				pos = Objs[i].transform.translation
				TranslationToRH &pos
				
				rot = Objs[i].rotation
				if (getUserProp Objs[i] "physics_generate") then
				(
					format "\t<instance_mesh name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"true\" physics_type=\"%\" physics_group=\"%\" visible=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "render_layer") (getUserProp Objs[i] "core_name") pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w (getUserProp Objs[i] "physics_generate_type") (getUserProp Objs[i] "physics_material") (getUserProp Objs[i] "physics_layer") true to:file
				) else (
					format "\t<instance_mesh name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" rotation=\"% % % %\" create_physics=\"false\" visible=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "render_layer") (getUserProp Objs[i] "core_name") pos[1] pos[2] pos[3] rot.x rot.y rot.z rot.w true to:file
				)
			)
		)
	)
	format "</renderable_objects>\n" to:file
	close file
)
