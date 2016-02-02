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
		local yaw
		local pitch
		local roll

		for l_Obj in Objs do
		(
			
			select l_Obj
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

		for l_Obj in Objs do
		(
			select l_Obj
			
			if (trackbar.getPreviousKeyTime()!=undefined) then
			(
				format "\t<cinematic_object resource=\"%\" renderable_objects_manager=\"%\">\n" l_Obj.name (getUserProp l_Obj "render_layer") to:file			
				
				local firstframe = at time 999999999999 trackbar.getNextKeyTime()
				local j = firstframe
				local keys = #()
				
				do (
					append keys j
					j = at time j trackbar.getNextKeyTime()
				) while j != firstframe
				
				for l_key in keys do
				(
					key = l_key as string	
					key = replace key key.count 1 ""
					key = (key as float)/30
				
					tform = at time l_key l_Obj.transform
					pos = tform.translation
					TranslationToRH &pos
					scl = tform.scale

					GetYaw tform &yaw
					GetPitch tform &pitch
					GetRoll tform &roll

					format "\t\t<cinematic_object_key_frame time=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" scale=\"% % %\"/>\n" key pos.x pos.y pos.z yaw pitch roll scl.x scl.y scl.z to:file
				)
				format "\t</cinematic_object>\n" to:file			
			)
		)
		format "</cinematic>\n" to:file
		close file
	)
)