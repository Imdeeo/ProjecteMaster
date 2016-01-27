fileIn "math.ms"
fileIn "utils.ms"

fn createInstanceMesh dir filename Objs=
(	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			if Objs[i].material != undefined then
			(
				pos = TranslationToRH Objs[i].transform.translation
				format "\t<renderable_object name=\"%\" core_name=\"%\" position=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "core_name") pos[1] pos[2] pos[3] (GetYaw Objs[i].transform) (GetPitch Objs[i].transform) (GetRoll Objs[i].transform) true to:file
			)
		)
	)
	format "</renderable_objects>\n" to:file
	close file
)
