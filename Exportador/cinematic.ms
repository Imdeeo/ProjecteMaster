fileIn "math.ms"
fileIn "utils.ms"

fn createCinematicObjects dir filename Objs isCal3D =
(	
	if Objs != undefined and Objs.count > 0 then
	(
		file=createfile (dir+filename)
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<cinematics>\n" to:file
		local key
		local obj_time
		local max_time
		local start_time
		local tform
		local rot
		local inversrot
		
		local l_Cinematics = #()
		append l_Cinematics (getUserProp Objs[1] "cinematic_name")
		
		for l_Obj in Objs do
		(
			l_New = true
			l_Prop = getUserProp l_Obj "cinematic_name"
			for l_Cinematic in l_Cinematics do
			(
				if l_Prop == l_Cinematic then l_New = false
			)
			if l_New then append l_Cinematics l_Prop
		)
		
		for l_Cinematic in l_Cinematics do
		(
			l_Objs = #()
			checkUserPropValue Objs "cinematic_name" l_Cinematic &l_Objs
			
			start_time=999999999
			max_time=0
			
			for l_Obj in l_Objs do
			(
				select l_Obj
				if (trackbar.getPreviousKeyTime()!=undefined) then
				(
					obj_time = (at time 999999999999 trackbar.getPreviousKeyTime())
					if obj_time > max_time then max_time = obj_time
					
					obj_time = (at time 999999999999 trackbar.getNextKeyTime())
					if obj_time < start_time then start_time = obj_time
				)
			)
			
			start_time = start_time as string
			start_time = replace start_time start_time.count 1 ""
			start_time = (start_time as float)/30
			
			max_time = max_time as string
			max_time = replace max_time max_time.count 1 ""
			max_time = (max_time as float)/30
			
			key = max_time - start_time
						
			format "\t<cinematic name=\"%\" duration=\"%\" cycle=\"false\">\n" l_Cinematic key to:file

			for l_Obj in l_Objs do
			(
				select l_Obj
				
				if (trackbar.getPreviousKeyTime()!=undefined) then
				(
					format "\t\t<cinematic_object resource=\"%\" layer=\"%\">\n" l_Obj.name (getUserProp l_Obj "render_layer") to:file			
					
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
						key = key - start_time
					
						tform = at time l_key l_Obj.transform
						pos = tform.translation
						--pos = pos*(rot as matrix3)
						pos.x = pos.x-l_Obj.transform.translation.x
						pos.y = pos.y-l_Obj.transform.translation.y
						pos.z = pos.z-l_Obj.transform.translation.z
						scl = tform.scale
						scl.x = scl.x/l_Obj.transform.scale.x
						scl.y = scl.y/l_Obj.transform.scale.y
						scl.z = scl.z/l_Obj.transform.scale.z
						if isCal3D then(
							CustomToRH &pos
						)else(
							TranslationToRH &pos
						)
						rot = l_Obj.transform.rotation
						local auxMat = rot as matrix3
						pos = pos*auxMat
						rot = tform.rotation
						--inversrot = l_Obj.transform.rotation
						--InverseQuat &inversrot
						--rot = rot * inversrot
						CustomQuatToRH &rot
						
						format "\t\t\t<cinematic_object_key_frame time=\"%\" pos=\"% % %\" rotation=\"% % % %\" scale=\"% % %\"/>\n" key pos.x pos.y pos.z rot.x rot.y rot.z rot.w scl.x scl.y scl.z to:file
					)
					format "\t\t</cinematic_object>\n" to:file			
				)
			)
			format "\t</cinematic>\n" to:file
		)
	
		format "</cinematics>\n" to:file
		close file
	)
)
