fileIn "math.ms"
fileIn "utils.ms"

fn createCameras dir Objs =
(
	file=createfile (dir+"cameras.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<cameras>\n" to:file
	format "\t<camera name=\"MainCamera\" type=\"fps\" target=\"Jaheem\" layer=\"solid\" offset=\"1.8\"/>\n" to: file
	format "\t<camera name=\"DebugCamera\" type=\"spherical\" distance=\"50\"/>\n" to: file
	if Objs != undefined then
	(
		local cameraFileDir
		local cameraTransform
		local cameraTranslation
		local cameraRotation
		local cameraPos
		local cameraUp
		local cameraLookAt
		local key
		local cameraType
		
		for i=1 to Objs.count do
		(
			select Objs[i]
			cameraTransform = Objs[i].transform
			cameraTranslation = cameraTransform.translation
			cameraRotation = cameraTransform as eulerangles
			local firstframe = at time 999999999999 trackbar.getNextKeyTime()
			if (firstframe != undefined) and ((getUserProp Objs[i] "camera_export_type") != "static") then (
				key = (at time 999999999999 trackbar.getPreviousKeyTime()) as string
				key = replace key key.count 1 ""
				key = (key as float)/30		
				dirExist (dir+"cameras\\")
				cameraFileDir = dir+"cameras\\"+Objs[i].name+".xml"
				cameraType = getUserProp Objs[i] "camera_export_type"
				TranslationToRH &cameraTranslation
				format "\t<camera name=\"%\" total_time=\"%\" type=\"key\" %=\"true\" pos=\"% % %\" filename=\"%\"/>\n" Objs[i].name key cameraType cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraFileDir to:file
				local j = firstframe
				cameraFile=createfile (dir+cameraFileDir)
				format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:cameraFile
				format "<camera_key_controller name=\"%\">\n" Objs[i].name to:cameraFile
				do (
					key = j as string	
					key = replace key key.count 1 ""
					key = (key as float)/30
					cameraTransform = at time j Objs[i].transform
					cameraTranslation = cameraTransform.translation
					cameraRotation = cameraTransform as eulerangles
					mat3xvect3 cameraTransform z_axis &cameraUp
					if(Objs[i].target != undefined) then
					(
						cameraLookAt = at time j Objs[i].target.pos
					) else (
						mat3xvect3 cameraTransform (-y_axis) &cameraLookAt
						swapYZ &cameraLookAt
						cameraLookAt += cameraTranslation
					)
					TranslationToRH &cameraUp
					TranslationToRH &cameraLookAt
					TranslationToRH &cameraTranslation
					format "\t<key time=\"%\" pos=\"% % %\" up=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraUp.x cameraUp.y cameraUp.z cameraLookAt.x cameraLookAt.y cameraLookAt.z (at time j Objs[i].fov) (at time j Objs[i].nearrange) (at time j Objs[i].farrange) to:cameraFile
					j = at time j trackbar.getNextKeyTime()
				) while j != firstframe
				format "</camera_key_controller>\n" to:cameraFile		
				close cameraFile
			) else (
				mat3xvect3 cameraTransform z_axis &cameraUp
				if(Objs[i].target != undefined) then
				(
					cameraLookAt = Objs[i].target.pos
				) else (
					mat3xvect3 cameraTransform (-y_axis) &cameraLookAt
					swapYZ &cameraLookAt
					cameraLookAt += cameraTranslation
				)
				cameraType = getUserProp Objs[i] "camera_export_type"
				TranslationToRH &cameraUp
				TranslationToRH &cameraLookAt
				TranslationToRH &cameraTranslation
				format "\t<camera name=\"%\" type=\"%\" pos=\"% % %\" up=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" Objs[i].name cameraType cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraUp.x cameraUp.y cameraUp.z cameraLookAt.x cameraLookAt.y cameraLookAt.z Objs[i].fov Objs[i].nearrange Objs[i].farrange to:file
			)
			max select none
		)		
	)
	format "</cameras>\n" to:file
	close file
)
