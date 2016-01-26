fileIn "Script.ms"

utility UABUtility "UABUtility" width:168 height:360
(
	checkbox chk1 "Checkbox" pos:[127,6] width:0 height:0
	button SetPathButton "Set Path" pos:[8,8] width:152 height:40
	label PathLabel "Label" pos:[8,56] width:152 height:64s
	checkbox ExportMeshCheck "Export Mesh" pos:[8,120] width:152 height:16
	checkbox ExportTexturesCheck "Export Textures" pos:[8,136] width:152 height:16
	groupBox grp1 "Export" pos:[8,176] width:152 height:176
	button ExportLightsButton "Lights" pos:[16,192] width:64 height:32
	button ExportMeshButton "Mesh" pos:[88,192] width:64 height:32
	button ExportCameraButton "Camera" pos:[16,232] width:64 height:32
	button ExportMaterialsButton "Materials" pos:[88,232] width:64 height:32
	button ExportStaticMeshButton "Static Mesh Manager" pos:[16,272] width:136 height:32
	button ExportSceneButton "Scene" pos:[16,312] width:136 height:32
	dropDownList LevelList "" pos:[8,152] width:152 height:21
	
	fn get_Dir =
	(
		local dir = ""+PathLabel.text+"\\" 
		dirExist dir
		dir += LevelList.items[LevelList.selection] + "\\"
		dirExist dir
		return dir
	)
	
	fn checkUserProp usrProp =
	(
		local usrDefined = #()
		for i = 1 to $*.count do
		(
			if (getUserProp $*[i] usrProp) == true then
			(
				local this_Obj = $*[i]
				append usrDefined $*[i]
			)
		)
		return usrDefined
	)
	
	fn exportLights dir =
	(
		local lightsToExport = checkUserProp "light_export"
		createLights dir "lights.xml" lightsToExport
	)

	fn exportCameras dir =
	(
		local camerasToExport = checkUserProp "camera_export"
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
				createMeshFromObjects dir Objs
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
		local materialsToExport = checkUserProp "render_export"
		createMaterials dir "materials.xml" materialsToExport textureCheck
	)

	fn exportStaticMesh dir meshCheck =
	(
		local meshToExport = checkUserProp "render_export"
		local coresToExport = get_Cores meshToExport
		createStaticMesh dir (LevelList.items[LevelList.selection]) "static_meshes.xml" coresToExport
		
		if meshCheck == true then 
		(
			createMeshFromObjects dir meshToExport
		)
	)

	fn exportRenderableObject dir =
	(
		local cinematicObjectsToExport = checkUserProp "cinematic_object"
		createCinematicObjects dir "cinematic.xml" cinematicObjectsToExport
		local renderableObjectsToExport = checkUserProp "render_export"
		createInstanceMesh dir "renderable_objects.xml" renderableObjectsToExport
	)

	on UABUtility open do
	(
		PathLabel.text="F:\\Game\\Data"
		
		local levels=#("","level_1","level_2", "level_3","level_4", "main_menu", "select_player")
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
		if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level!" title:"ERROR"
        )
		else 
		(
			exportLights (get_Dir())
		)
	)
	
	on ExportMeshButton pressed do
    (        
        if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level!" title:"ERROR"
        )
        else
        (
            if $ == undefined then
            (
                messageBox "You must select a scene object" title:"ERROR"
            )
            else
            (
                exportMesh (get_Dir()) $
            )
        )
    )
	
	on ExportCameraButton pressed do
	(
		if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level!" title:"ERROR"
        )
		else 
		(
			exportCameras (get_Dir())
		)
	)
	
	on ExportMaterialsButton pressed do
	(
		if LevelList.items[LevelList.selection] == "" then
		(			
			messageBox "You must select a level" title:"ERROR"
		)
		else
		(
			exportMaterials (get_Dir()) (ExportTexturesCheck.checked)
		)
	)
	
	on ExportStaticMeshButton pressed do
    (
        if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level!" title:"ERROR"
        )
        else
        (
            exportStaticMesh (get_Dir()) (ExportMeshCheck.checked)
        )
    )
	
	on ExportSceneButton pressed do
	(
		 if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level!" title:"ERROR"
        )
        else
        (
			local dir = get_Dir()
			exportLights dir
			exportCameras dir
			exportMaterials dir (ExportTexturesCheck.checked)
			exportStaticMesh dir (ExportMeshCheck.checked)
			exportRenderableObject dir
		)
	)
)

--Level -> Carpeta data con los niveles
--Lights -> Exporta las luces
--Mesh -> Exporta el binario de mesh
--Camera -> Exporta xml de cmaras
--Materials -> Exporta xml de materiales, si está export textures marcado
--StaticMeshManager -> Genera xml de staticmesh, si está mesh marcado, tambien el binario
--Scene -> Genera xml de materials, staticmesh y renderableobjects, y las luces y camaras, usando las casillas