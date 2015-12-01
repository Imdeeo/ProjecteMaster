utility UABUtility "UABUtility" width:168 height:360
(
	checkbox chk1 "Checkbox" pos:[127,6] width:0 height:0
	button SetPathButton "Set Path" pos:[8,8] width:152 height:40
	label PathLabel "Label" pos:[8,56] width:152 height:64
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

	on UABUtility open do
	(
		PathLabel.text="C:\\UABEngine\\Data"
		
		local levels=#("","level 1","level 2", "level 3","level 4", "main menu", "select player")
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
	
	on ExportMaterialsButton pressed do
	(
		if LevelList.items[LevelList.selection] == "" then
		(			
			messageBox "You must select a level" title:"ERROR"
		)
		else
		(
			local dir = PathLabel.text + "\\" + LevelList.items[LevelList.selection] + "\\"
			exportMaterials dir ExportTexturesCheck.checked
		)
	)
	
	on ExportStaticMeshButton pressed do
    (
        if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level" title:"ERROR"
        )
        else
        (
			local dir = PathLabel.text + "\\" + LevelList.items[LevelList.selection] + "\\"
            exportStaticMesh dir ExportMeshCheck.checked
        )
    )
    
    on ExportMeshButton pressed do
    (        
        if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level" title:"ERROR"
        )
        else
        (
            if $ == undefined then
            (
                messageBox "You must select a scene object" title:"ERROR"
            )
            else
            (
                local l_directoriMesh = PathLabel.text + "\\" + LevelList.items[LevelList.selection] + "\\Meshes\\"
				dirExist l_directoriMesh
                file=fopen (l_directoriMesh+$.name+".mesh") "wb"				
                writeMeshFromObject file ($)
                FClose file
            )
        )
    )
	
	on ExportSceneButton pressed do
	(
		 if LevelList.items[LevelList.selection] == "" then
        (
            messageBox "You must select a level" title:"ERROR"
        )
        else
        (
			local dir = PathLabel.text + "\\" + LevelList.items[LevelList.selection] + "\\"
			exportMaterials dir ExportTexturesCheck.checked
			exportStaticMesh dir ExportMeshCheck.checked
			exportRenderableObject dir
		)
	)
)

fn exportMaterials dir textureCheck =
(
	createMaterials  dir "materials.xml" (get_Cores()) textureCheck
)

fn exportStaticMesh dir meshCheck =
(
	createStaticMesh dir "static_meshes.xml" (get_Instances())
	
	if meshCheck == true then 
	(				
		local l_directoriMeshes = dir + "Meshes\\"
		createMeshFromObjects l_directoriMeshes (get_Instances())
	)
)

fn exportRenderableObject dir =
(
	createInstanceMesh dir "renderable_objects.xml" (get_Instances())
)

--Level -> Carpeta data con los niveles
--Lights -> Exporta las luces
--Mesh -> Exporta el binario de mesh
--Camera -> Exporta xml de cmaras
--Materials -> Exporta xml de materiales, si está export textures marcado
--StaticMeshManager -> Genera xml de staticmesh, si está mesh marcado, tambien el binario
--Scene -> Genera xml de materials, staticmesh y renderableobjects, y las luces y camaras, usando las casillas