fileIn "camera.ms"
fileIn "cinematic.ms"
fileIn "export.ms"
fileIn "light.ms"
fileIn "renderable_object.ms"
fileIn "static_mesh.ms"

utility UABUtility "UABUtility" width:168 height:360
(
	checkbox chk1 "Checkbox" pos:[127,6] width:0 height:0
	button SetPathButton "Set Path" pos:[8,8] width:152 height:40
	label PathLabel "Label" pos:[8,56] width:152 height:64s
	checkbox ExportMeshCheck "Export Mesh" pos:[8,120] width:152 height:16
	checkbox ExportTexturesCheck "Export Textures" pos:[8,136] width:152 height:16
    groupBox grp1 "Export" pos:[8,176] width:152 height:221
	button ExportLightsButton "Lights" pos:[16,192] width:64 height:32
	button ExportMeshButton "Mesh" pos:[88,192] width:64 height:32
	button ExportCameraButton "Camera" pos:[16,232] width:64 height:32
	button ExportMaterialsButton "Materials" pos:[88,232] width:64 height:32
	button ExportStaticMeshButton "Static Mesh Manager" pos:[16,272] width:136 height:32
	button ExportSceneButton "Scene" pos:[16,312] width:136 height:32
	dropDownList LevelList "" pos:[8,152] width:152 height:21
	label progLabel "Progress bar" pos:[16,352] width:140 height:20
    progressbar prog color:gray pos:[16,372]

	fn get_Dir =
	(
		local dir = ""+PathLabel.text+"\\" 
		dirExist dir
		dir += LevelList.items[LevelList.selection] + "\\"
		dirExist dir
		dir
	)
	
	fn exportLights dir =
	(
		local lightsToExport = #()
		checkUserProp "light_export" &lightsToExport
		createLights dir "lights.xml" lightsToExport
	)

	fn exportCameras dir =
	(
		local camerasToExport = #()
		checkUserProp "camera_export" &camerasToExport
		createCameras dir camerasToExport
	)

	fn exportMesh dir Obj =
	(
		local Objs = #()
		
		if ClassOf Obj != ObjectSet then
		(
			if (SuperClassOf  Obj) == GeometryClass then
			(
				append Objs Obj
				createMeshFromObjects dir Objs &prog &progLabel
			)
			else
			(
				messageBox "You must select a geometry!" title:"ERROR"
			)
		)
		else
		(
			local nonGeometry = false
			for i=1 to Obj.count do
			(
				if SuperClassOf Obj[i]  == GeometryClass then
				(
					append Objs Obj[i]
				)
				else
				(
					nonGeometry = true
				)
			)
			if nonGeometry == false then
			(
				createMeshFromObjects dir Objs
			)
			else
			(
				messageBox "You have selected one or more non-geometry objects!" title:"ERROR"
			)
		)
	)

	fn exportMaterials dir textureCheck =
	(
		local materialsToExport = #()
		checkUserProp "render_export" &materialsToExport
		createMaterials dir "materials.xml" materialsToExport textureCheck
	)

	fn exportStaticMesh dir meshCheck =
	(
		local meshToExport = #()
		checkUserProp "render_export" &meshToExport
		local coresToExport = #()
		getCores meshToExport &coresToExport
		createStaticMesh dir (LevelList.items[LevelList.selection]) "static_meshes.xml" coresToExport
		if meshCheck == true then 
		(
			createMeshFromObjects dir meshToExport &prog &progLabel
		)
	)

	fn exportRenderableObject dir =
	(
		local cinematicObjectsToExport = #()
		checkUserProp "cinematic_object" &cinematicObjectsToExport
		createCinematicObjects dir "cinematic.xml" cinematicObjectsToExport
		local renderableObjectsToExport = #()
		checkUserProp "render_export" &renderableObjectsToExport
		createInstanceMesh dir "renderable_objects.xml" renderableObjectsToExport
	)
	
	on UABUtility open do
	(
		PathLabel.text="F:\\Game\\Data"
		
		local levels=#("level_1","level_2", "level_3","level_4", "main_menu", "select_player")
		LevelList.items=levels		
	)
	
	on SetPathButton pressed do
	(
		local l_Path=getSavePath caption:"Select game folder" initialDir:PathLabel.text
		if l_Path!=undefined then
		(
			PathLabel.text=l_Path
		)
	)
	
	on ExportLightsButton pressed do
	(
		checkForSave() 
		exportLights (get_Dir())
		resetMaxFile() #noPrompt
	)
	
	on ExportMeshButton pressed do
    (        
		if $ == undefined then
		(
			messageBox "You must select a scene object" title:"ERROR"
		)
		else
		(
			checkForSave() 
			exportMesh (get_Dir()) $
			resetMaxFile() #noPrompt
		)
    )
	
	on ExportCameraButton pressed do
	(
		checkForSave() 
		exportCameras (get_Dir())
		resetMaxFile() #noPrompt
	)
	
	on ExportMaterialsButton pressed do
	(
		checkForSave() 
		exportMaterials (get_Dir()) (ExportTexturesCheck.checked)
		resetMaxFile() #noPrompt
	)
	
	on ExportStaticMeshButton pressed do
    (
		checkForSave() 
		exportStaticMesh (get_Dir()) (ExportMeshCheck.checked)
		resetMaxFile() #noPrompt
    )
	
	on ExportSceneButton pressed do
	(
		--checkForSave() 
		undo off(
			local theWatch = PerformanceTester()
			theWatch.start()
			local dir = get_Dir()
			exportLights dir
			exportCameras dir
			exportMaterials dir (ExportTexturesCheck.checked)
			exportStaticMesh dir (ExportMeshCheck.checked)
			exportRenderableObject dir
			prog.value = 0
			progLabel.text = "Done!"
			theWatch.stop()
			theWatch.report()
		)
		--resetMaxFile()
	)
)
