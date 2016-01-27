fileIn "math.ms"
fileIn "utils.ms"

fn createCinematicObjects dir filename Objs =
(	
	if Objs != undefined then
	(
		file=createfile (dir+filename)
		local key
		local obj_time
		local max_time=0
		local tform
		
		for i=1 to Objs.count do
		(
			select Objs[i]
			if (trackbar.getPreviousKeyTime()!=undefined) then
			(
				obj_time = (at time 999999999999 trackbar.getPreviousKeyTime())
				if obj_time > max_time then
				(
					max_time = obj_time
				)	
			)
		)
		
		key = max_time as string
		key = replace key key.count 1 ""
		key = (key as float)/30
			
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<cinematic name=\"%\" duration=\"%\">\n" maxFileName key to:file
		
		for i=1 to Objs.count do
		(
			select Objs[i]
			
			if (trackbar.getPreviousKeyTime()!=undefined) then
			(
				format "\t<cinematic_object resource=\"%\" renderable_objects_manager=\"%\">\n" Objs[i].name (getUserProp Objs[i] "render_layer") to:file			
				
				local firstframe = at time 999999999999 trackbar.getNextKeyTime()
				local j = firstframe
				do (
					key = j as string	
					key = replace key key.count 1 ""
					key = (key as float)/30
					
					tform = at time j Objs[i].transform
					
					pos = TranslationToRH tform.translation
					scl = tform.scale
						
					format "\t\t<cinematic_object_key_frame time=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" scale=\"% % %\"/>\n" key pos[1] pos[2] pos[3] (GetYaw tform) (GetPitch tform) (GetRoll tform) scl[1] scl[2] scl[3] to:file
						
					j = at time j trackbar.getNextKeyTime()
				) while j != firstframe
				
				format "\t</cinematic_object>\n" to:file			
			)
		)
		format "</cinematic>\n" to:file
		close file
	)
)
