fileIn "math.ms"
fileIn "utils.ms"

fn createInstanceMesh dir filename Objs=
(	
	local pos
	local yaw
	local pitch
	local roll
	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	format "\t<layer name=\"solid\" default=\"true\"/>\n" to:file
	format "\t<layer name=\"alpha_objects\"/>\n" to:file
	format "\t<layer name=\"alpha_blend_objects\"/>\n" to:file
	format "\t<layer name=\"particles\"/>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			if Objs[i].material != undefined then
			(
				pos = Objs[i].transform.translation
				TranslationToRH &pos
				
				GetYaw Objs[i].transform &yaw
				GetPitch Objs[i].transform &pitch
				GetRoll Objs[i].transform &roll
				
				format "\t<renderable_object name=\"%\" layer =\"%\" core_name=\"%\" position=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "layer") (getUserProp Objs[i] "core_name") pos[1] pos[2] pos[3] yaw pitch roll true to:file
			)
		)
	)
	format "</renderable_objects>\n" to:file
	close file
)
