fileIn "math.ms"
fileIn "utils.ms"

global g_Path
	
rollout CamAnim "Camera Animation" width:160 height:134
(
	label CameraLabel "Nothing selected" pos:[24,12] width:232 height:24 visible:true
	spinner CameraKeyStart pos:[24,48] width:42 range:[0,1000,0] type:#integer scale:1 enabled:false visible:true
	spinner CameraKeyEnd pos:[74,48] width:42 range:[0,1000,0] type:#integer scale:1 enabled:false visible:true
	checkbox NegateCheck "" pos:[131,48] width:16 height:16 checked:false
	label CameraLabelStart "start" pos:[31,32] width:42 height:12 enabled:false visible:true
	label CameraLabelEnd "end" pos:[82,32] width:42 height:12 enabled:false visible:true
	label NegateLabel "-1" pos:[139,37] width:42 height:12 enabled:false visible:true 
	edittext CameraAnimationName "Name: " pos:[24,74] width:112 height:16 enabled:false visible:true
	button ExportButton "Export" pos:[24,104] width:112 height:16 visible:true
	
	fn loadOK Obj =
	(
		CameraLabel.text = Obj.name
		CameraKeyStart.enabled = true
		CameraKeyEnd.enabled = true
		CameraLabelStart.enabled = true
		CameraLabelEnd.enabled = true
		CameraAnimationName.enabled = true
		NegateCheck.enabled = true
		NegateLabel.enabled = true
		ExportButton.enabled = true
	)
	
	fn loadFAIL Obj =
	(
		if (Obj == undefined) then
		(
			CameraLabel.text = "Nothing selected."
		) else (
			CameraLabel.text = "Selection not 1 camera."
		)
		CameraKeyStart.enabled = false
		CameraKeyEnd.enabled = false
		CameraLabelStart.enabled = false
		CameraLabelEnd.enabled = false
		CameraAnimationName.enabled = false
		NegateCheck.enabled = false
		NegateLabel.enabled = false
		ExportButton.enabled = false
	)
	
	fn checkCam Obj =
	(
		case SuperClassOf Obj of
		(
			camera: loadOK Obj
			default: loadFAIL Obj
		)
	)
	
	fn AddCameraAnimation _file =
	(
		local Cam = $
		local key = (CameraKeyEnd.value as float - CameraKeyStart.value as float)/30
		local j = CameraKeyStart.value as time
		local cameraTransform = Cam.transform
		local cameraTranslation = cameraTransform.translation
		local cameraRotation
		local cameraLookAt
		local cameraFieldOfView
        local cameraFileDir = g_Path+"cameras\\"+CameraAnimationName.text+".xml"
        format "\t<camera name=\"%\" total_time=\"%\" type=\"key\" %=\"true\" pos=\"% % %\" filename=\"%\"/>\n" CameraAnimationName.text key cameraType cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraFileDir to:_file
		
		cameraFile = createfile cameraFileDir
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:cameraFile
		format "<camera_key_controller name=\"%\">\n" CameraAnimationName.text to:cameraFile		
		do (
			key = j as string	
			key = replace key key.count 1 ""
			key = ((key as float) - CameraKeyStart.value)/30
			cameraTransform = at time j Cam.transform
			cameraTranslation = cameraTransform.translation
			checkZeroes &cameraTranslation
			if NegateCheck.checked then
			(
				rotateX cameraTransform 90
			) else (
				rotateX cameraTransform -90
			)
			mat3xvect3 cameraTransform z_axis &cameraUp
			checkZeroes &cameraUp
			if(Cam.target != undefined) then
			(
				select Cam.target
				cameraLookAt = at time j Cam.target.pos
			) else (	
				mat3xvect3 cameraTransform y_axis &cameraLookAt
				if not NegateCheck.checked then
				(
					cameraLookAt.z = -cameraLookAt.z
				)
				checkZeroes &cameraLookAt
				cameraLookAt += cameraTranslation
			)
			TranslationToRH &cameraUp
			TranslationToRH &cameraLookAt
			TranslationToRH &cameraTranslation
			if NegateCheck.checked then
			(
				negateXZ &cameraUp
				negateXZ &cameraLookAt
				negateXZ &cameraTranslation
			)
			cameraFieldOfView = at time j Cam.fov
			verticalizeFov &cameraFieldOfView
			deg2rad &cameraFieldOfView
			format "\t<key time=\"%\" pos=\"% % %\" up=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraUp.x cameraUp.y cameraUp.z cameraLookAt.x cameraLookAt.y cameraLookAt.z cameraFieldOfView (at time j Cam.nearrange) (at time j Cam.farrange) to:cameraFile
			j = at time j trackbar.getNextKeyTime()
		) while j < CameraKeyEnd.value
		
		j = CameraKeyEnd.value as time
		key = ((CameraKeyEnd.value as float) - CameraKeyStart.value)/30
		cameraTransform = at time j Cam.transform
		cameraTranslation = cameraTransform.translation
		checkZeroes &cameraTranslation
		if NegateCheck.checked then
		(
			rotateX cameraTransform 90
		) else (
			rotateX cameraTransform -90
		)
		mat3xvect3 cameraTransform z_axis &cameraUp
		checkZeroes &cameraUp
		if(Cam.target != undefined) then
		(
			select Cam.target
			cameraLookAt = at time j Cam.target.pos
		) else (	
			mat3xvect3 cameraTransform y_axis &cameraLookAt
			if not NegateCheck.checked then
			(
				cameraLookAt.z = -cameraLookAt.z
			)
			checkZeroes &cameraLookAt
			cameraLookAt += cameraTranslation
		)
		TranslationToRH &cameraUp
		TranslationToRH &cameraLookAt
		TranslationToRH &cameraTranslation
		if NegateCheck.checked then
		(
			negateXZ &cameraUp
			negateXZ &cameraLookAt
			negateXZ &cameraTranslation
		)
		cameraFieldOfView = at time j Cam.fov
		verticalizeFov &cameraFieldOfView
		deg2rad &cameraFieldOfView
		format "\t<key time=\"%\" pos=\"% % %\" up=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraUp.x cameraUp.y cameraUp.z cameraLookAt.x cameraLookAt.y cameraLookAt.z cameraFieldOfView (at time j Cam.nearrange) (at time j Cam.farrange) to:cameraFile
			
		format "</camera_key_controller>\n" to:cameraFile
		close cameraFile
		select Cam
	)
	
	on CamAnim open do
	(
		checkCam $
	)
	
	on CamAnim close do
	(
		callbacks.removeScripts #selectionSetChanged id:#checkCam
	)
	
	on ExportButton pressed do
	(
		dirExist g_Path
        dirExist (g_Path+"cameras\\")
		local file
        if existFile (g_Path+"cameras.xml") then
		(
            file=openFile (g_Path+"cameras.xml") mode:"r+"
			seek file #eof
			seek file (filepos file - 12)
			AddCameraAnimation file
			format "</cameras>\n" to:file
			close file
		) else (
            file=createFile (g_Path+"cameras.xml")
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
			format "<cameras>\n" to:file
			AddCameraAnimation file
			format "</cameras>\n" to:file
			close file
		)
	)
)

--createDialog CamAnim
