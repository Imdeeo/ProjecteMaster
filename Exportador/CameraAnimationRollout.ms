fileIn "math.ms"
fileIn "utils.ms"

Obj = undefined
callbacks.removeScripts #selectionSetChanged id:#checkCam
callbacks.addScript #selectionSetChanged "CamAnim.checkCam $" id:#checkCam persistent:false
global g_Path = "C:\\Users\\Public\\"
	
rollout CamAnim "Camera Animation" width:160 height:134
(
	label CameraLabel "Nothing selected" pos:[24,12] width:232 height:24 visible:true
	spinner CameraKeyStart pos:[24,48] width:42 range:[0,1000,0] type:#integer scale:1 enabled:false visible:true
	spinner CameraKeyEnd pos:[94,48] width:42 range:[0,1000,0] type:#integer scale:1 enabled:false visible:true
	label CameraLabelStart "start" pos:[31,32] width:42 height:12 enabled:false visible:true
	label CameraLabelEnd "end" pos:[102,32] width:42 height:12 enabled:false visible:true
	edittext CameraAnimationName "Name: " pos:[24,74] width:112 height:16 enabled:false visible:true
	button SetPathButton "Set Path" pos:[24,96] width:112 height:16 visible:true
	button ExportButton "Export" pos:[24,114] width:112 height:16 visible:true
	
	fn existFile fname = (getfiles fname).count != 0
	
	fn loadOK Obj =
	(
		CameraLabel.text = Obj.name
		CameraKeyStart.enabled = true
		CameraKeyEnd.enabled = true
		CameraLabelStart.enabled = true
		CameraLabelEnd.enabled = true
		CameraAnimationName.enabled = true
		SetPathButton.enabled = true
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
		SetPathButton.enabled = false
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
		local key = (CameraKeyEnd.value as float - CameraKeyStart.value as float)/30
		format "\t<camera_animation name=\"%\" duration=\"%\">\n" CameraAnimationName.text key to:_file
		local j = CameraKeyStart.value as time
		local cameraTransform
		local cameraTranslation
		local cameraRotation
		local cameraLookAt
		do (
			key = j as string	
			key = replace key key.count 1 ""
			key = ((key as float) - CameraKeyStart.value)/30
			cameraTransform = at time j $.transform
			cameraTranslation = cameraTransform.translation
			cameraRotation = cameraTransform as eulerangles
			mat3xvect3 cameraTransform z_axis &cameraUp
			if($.target != undefined) then
			(
				cameraLookAt = at time j $.target.pos
			) else (
				mat3xvect3 cameraTransform (-y_axis) &cameraLookAt
				swapYZ &cameraLookAt
				cameraLookAt += cameraTranslation
			)
			TranslationToRH &cameraUp
			TranslationToRH &cameraLookAt
			TranslationToRH &cameraTranslation
			format "\t\t<camera_animation_key time=\"%\" pos=\"% % %\" up=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraTranslation.x cameraTranslation.y cameraTranslation.z cameraUp.x cameraUp.y cameraUp.z cameraLookAt.x cameraLookAt.y cameraLookAt.z (at time j $.fov) (at time j $.nearrange) (at time j $.farrange) to:_file
			j = at time j trackbar.getNextKeyTime()
		) while j <= CameraKeyEnd.value
		format "\t</camera_animation>\n" to:_file
	)
	
	on SetPathButton pressed do
	(
		local l_Path=getSavePath caption:"Select file folder" initialDir:g_Path
		if l_Path!=undefined then
		(
			g_Path=l_Path+"\\"
		)
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
		local file
		if existFile (g_Path+"camera_animation.xml") then
		(
			file=openFile (g_Path+"camera_animation.xml") mode:"r+"
			seek file #eof
			seek file (filepos file - 22)
			AddCameraAnimation file
			format "</camera_animations>\n" to:file
			close file
		) else (
			file=createFile (g_Path+"camera_animation.xml")
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
			format "<camera_animations>\n" to:file
			AddCameraAnimation file
			format "</camera_animations>\n" to:file
			close file
		)
	)
)
createDialog CamAnim