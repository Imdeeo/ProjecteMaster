fileIn "math.ms"
fileIn "utils.ms"

fn createLights dir filename Objs =
(
	local lightPos
	local lightDir
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<lights>\n" to:file
	format "\t<ambient_light color=\"0.1 0.1 0.1 1.0\"/>\n" to:file
	
	for i=1 to Objs.count do
	(
		lightPos = TranslationToRH Objs[i].pos
		lightDir = TranslationToRH Objs[i].dir
		if (ClassOf Objs[i] == Omnilight) then
		(
			format "\t<light name=\"%\" enabled=\"%\" type=\"omni\" pos=\"% % %\" color=\"% % % %\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "light_enabled") lightPos.x lightPos.y lightPos.z (Objs[i].color.r/255) (Objs[i].color.g/255) (Objs[i].color.b/255) 1 Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		) 
		else if (ClassOf Objs[i] == Directionallight or ClassOf Objs[i] == TargetDirectionallight) then
		(
			format "\t<light name=\"%\" enabled=\"%\" type=\"directional\" pos=\"% % %\" dir=\"% % %\" color=\"% % % %\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "light_enabled") lightPos.x lightPos.y lightPos.z lightDir.x lightDir.y lightDir.z (Objs[i].color.r/255) (Objs[i].color.g/255) (Objs[i].color.b/255) 1 Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		) 
		else if (ClassOf Objs[i] == freeSpot or ClassOf Objs[i] == targetSpot) then
		(
			format "\t<light name=\"%\" enabled=\"%\" type=\"spot\" pos=\"% % %\" dir=\"% % %\" color=\"% % % %\" angle=\"%\" fall_off=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" Objs[i].name (getUserProp Objs[i] "light_enabled") lightPos.x lightPos.y lightPos.z lightDir.x lightDir.y lightDir.z (Objs[i].color.r/255) (Objs[i].color.g/255) (Objs[i].color.b/255) 1 Objs[i].hotspot Objs[i].falloff Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		)
	)
	format "</lights>\n" to:file		
	close file
)
