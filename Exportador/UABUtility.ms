global isDebug = false

fn get_LocalFileName =
(
	return "Game\\Data\\"
)

fn get_FileName =
(
	return "E:\\" + (get_LocalFileName())
)

fn get_Cores =
(
	return $*
)

fn get_Instances =
(
	return $*
)

fn createStaticMesh dir filename Objs=
(
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	for i=1 to Objs.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%\"/>\n" Objs[i].name ((get_LocalFileName())+"Meshes\\"+Objs[i].name+".mesh") to:file
	)
	format "</static_meshes>\n" to:file
	close file
)

fn TranslationToRH translation =
(
	return point3 -translation.x translation.z translation.y
)

fn GetYaw transform =
(
	return -(transform as eulerangles).z
)

fn GetPitch transform =
(
	return -(transform as eulerangles).y
)

fn GetRoll transform =
(
	return (transform as eulerangles).x
)

fn createInstanceMesh dir filename Objs=
(
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	for i=1 to Objs.count do
	(
		Obj = Objs[i]
		pos = TranslationToRH Obj.transform.translation
		format "\t<renderable_object name=\"%\" core_name=\"%\" position=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n" Obj.name Obj.name pos[1] pos[2] pos[3] (GetYaw Obj.transform) (GetPitch Obj.transform) (GetRoll Obj.transform) true to:file
	)
	format "</renderable_objects>\n" to:file
	close file
)

fn writeHeader arx =
(
	WriteShort arx 0xFE55
)

fn writeFooter arx =
(
	WriteShort arx 0x55FE
)

fn getMaterials arx Obj =
(
	local n_materials=1
	if(classof(Obj.material)==Multimaterial)then
	(
		if isDebug then
		format "Multimaterial\n"
		n_materials=Obj.material.numsubs
		writeLong arx n_materials
		for i=1 to Obj.material.numsubs do
		(
			WriteShort arx Obj.material[i].name.count #unsigned
			writeString arx Obj.material[i].name
		)
	)
	else
	(
		if isDebug then
		format "Single Material\n"
		writeLong arx 1
		WriteShort arx Obj.material.name.count #unsigned
		writeString arx Obj.material.name
	)
	return n_materials
)

fn equalVertex vt1 vt2 =
(
	local equals = true
	if vt1.count==vt2.count then
	(
		for i=1 to vt1.count do
		(
			if(vt1[i]!=vt2[i]) then
			(
				equals = false
			)
		)
	)
	else
	(
		equals = false
	)
	return equals
)

fn getIndex vertexVect vertex IdMaterial =
(
	local l_FoundVertex=false
	local index = (vertexVect[IdMaterial]).count
	for i=1 to vertexVect[IdMaterial].count do
	(
		if (equalVertex vertexVect[IdMaterial][i] vertex) then
		(
			index = i-1
			l_FoundVertex=true
		)
	)
	if l_FoundVertex==false then
	(
		index=vertexVect[IdMaterial].count
		append vertexVect[IdMaterial] vertex
	)
	return index
)

fn writeMesh arx vertexs indexs =
(
	if isDebug then
	format "Number of materials: %\n" vertexs.count
	for i=1 to vertexs.count do
	(
		if isDebug then
		format "Material %:\n" i
		writeShort arx 37 #unsigned
		if isDebug then
		format "Size of vertex: 37\n"
		writeLong arx vertexs[i].count
		if isDebug then
		format "Number of vertexes: %\n" vertexs[i].count
		for j=1 to vertexs[i].count do
		(
			if isDebug then
			format "Vertex %: " j
			for k=1 to vertexs[i][j].count do
			(
				writeFloat arx vertexs[i][j][k]
				if isDebug then
				format "% "vertexs[i][j][k]
			)
			if isDebug then
			format "\n"
		)
		writeShort arx 16 #unsigned
		if isDebug then
		format "Size of index: 16\n"
		writeLong arx indexs[i].count
		if isDebug then
		format "Number of indexes: %\n" indexs[i].count
		for j=1 to indexs[i].count do
		(
			writeShort arx indexs[i][j] #unsigned
			if isDebug then
			format "Index %: %\n" j indexs[i][j]
		)
	)
)

fn createVertex pos norm textcoord =
(
	local vertex = #()
	append vertex pos.x
	append vertex pos.y
	append vertex pos.z
	append vertex norm.x
	append vertex norm.y
	append vertex norm.z
	append vertex textcoord.x
	append vertex textcoord.y
	return vertex
)

fn procesFaces arx Obj =
(
	local l_Obj=Obj
	local l_ObjectConverted=false
	if classof l_Obj!=Editable_mesh then
	(
		l_Obj=copy Obj
		convertToMesh l_Obj
		l_ObjectConverted=true
	)
	
	local nFaces = getNumFaces l_Obj
	local vertexs = #()
	local indexs = #()
	local l_HasMultimaterial=classof l_Obj.material==Multimaterial
	local l_TotalMaterials=1
	if l_HasMultimaterial then
		l_TotalMaterials=l_Obj.material.count
	
	for i=1 to l_TotalMaterials do
	(
		append vertexs #()
		append indexs #()
	)	
	
	for i=1 to nFaces do
	(
		local IdxsFace=getFace l_Obj i
		local Vt1=getVert l_Obj IdxsFace.x
		local Vt2=getVert l_Obj IdxsFace.y
		local Vt3=getVert l_Obj IdxsFace.z
		local Nm1=getNormal l_Obj IdxsFace.x
		local Nm2=getNormal l_Obj IdxsFace.y
		local Nm3=getNormal l_Obj IdxsFace.z
		local IdxsMap=meshop.getMapFace l_Obj 1 i
		local TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap.x
		local TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap.y
		local TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap.z
		local v1 = createVertex Vt1 Nm1 TUVMap1
		local v2 = createVertex Vt2 Nm2 TUVMap2
		local v3 = createVertex Vt3 Nm3 TUVMap3
		
		local l_IdMaterial=getFaceMatID l_Obj i
		if l_HasMultimaterial==false then
		(
			l_IdMaterial=1
		)
		
		local id1=getIndex vertexs v1 l_IdMaterial
		local id2=getIndex vertexs v2 l_IdMaterial
		local id3=getIndex vertexs v3 l_IdMaterial
		
		if isDebug then
		(format "---------FACE %-----------\n" i
		format "Vertex 1: % -> Index: %\n" v1 id1
		format "Vertex 2: % -> Index: %\n" v2 id2
		format "Vertex 3: % -> Index: %\n" v2 id3
		format "---------------------------\n\n"
		)
		append indexs[l_IdMaterial] id1
		append indexs[l_IdMaterial] id2
		append indexs[l_IdMaterial] id3
	)
	writeMesh arx vertexs indexs
	
	if l_ObjectConverted==true then
	(
		delete l_Obj
	)
)

fn writeMeshFromObject arx Obj =
(
	writeHeader arx
	local n_materials = getMaterials arx Obj
	for i=1 to n_materials do
	(
		procesFaces arx Obj
	)
	writeFooter arx
)

fn createMeshFromObjects Dir Objs =
(
	for i=1 to Objs.count do
	(
		file=fopen (Dir+Objs[i].name+".mesh") "wb"
		writeMeshFromObject file (Objs[i])
		FClose file
	)
)

fn createMaterials dir filename Objs=
(
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	materialsDictionary = dotNetObject "System.Collections.Hashtable"
	for i=1 to Objs.count do
	(
		mat = #()
		n_materials = 1
		if(classof(Objs[i].material)==Multimaterial)then
		(
			mat = Objs[i].material
			n = Objs[i].material.numsubs
		)
		else
		(			
			append mat Objs[i].material
		)	
		for j=1 to n_materials do
			(
				if materialsDictionary.Item[mat[j].name]==undefined then
				(
					format "\t<material name=\"%\" effect_technique=\"diffuse_technique\">\n" mat[j].name to:file
					materialsDictionary.Add mat[j].name "defined"
					if mat[j].diffuseMap != undefined then  
					(
						if (classof mat[j].diffuseMap == Bitmaptexture) and (mat[j].diffuseMap.bitmap != undefined) then
						(
							nomTextura = filenameFromPath (mat[j].diffuseMap.bitmap.filename)
							
								pathfile = ((get_FileName())  + "Textures\\" + nomTextura)
								format "\t\t<texture filename=\"%\"/>\n" pathfile to:file
						)
					)
					format "\t</material>\n" to:file 
				)
			)
	)		
	format "</materials>\n" to:file		
	close file
)

fn createFiles Dir Objs =
(
	createInstanceMesh Dir "renderable_objects.xml" Objs
	createStaticMesh Dir "static_meshes.xml" Objs
	createMaterials Dir "materials.xml" Objs
	createMeshFromObjects Dir Objs
)


utility UABUtility "UABUtility" width:168 height:360
(
	checkbox chk1 "Checkbox" pos:[127,6] width:0 height:0
	button SetPathButton "Set Path" pos:[8,8] width:152 height:40
	label PathLabel "Label" pos:[8,56] width:152 height:64
	checkbox ExportMeshCheck "Export Mesh" pos:[8,120] width:152 height:16
	checkbox ExportTexturesBox "Export Textures" pos:[8,136] width:152 height:16
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
		PathLabel.text="D:\\UABEngine\\Data"
		
		local levels=#("","level1","level2", "level3","level4", "main_menu", "select_player")
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
	
	on ExportStaticMeshButton pressed do
	(
		
		--createStaticMesh (get_FileName () + LevelList  "static_meshes.xml" (get_Cores())
		--ExportMeshCheck.checked
		if LevelList.items[LevelList.selection] == "" then
		(
			messageBox "Introduce un level" title:"ERROR"
		)
		else
		(
			local l_directori = (get_FileName() + LevelList.items[LevelList.selection] +"\\")
			makeDir l_directori all: true
			createStaticMesh l_directori "static_meshes.xml" (get_Instances())
			if ExportMeshCheck.checked then 
			(
				local l_directoriMeshes = l_directori + "Meshes\\"
				makeDir l_directoriMeshes
				createMeshFromObjects l_directoriMeshes (get_Instances())
			)
		)
	)
	
	on ExportMeshButton pressed do
	(		
		if LevelList.items[LevelList.selection] == "" then
		(
			messageBox "Introduce un level" title:"ERROR"
		)
		else
		(
			if $ == undefined then
			(
				messageBox "Selecciona un objeto de la escena" title:"ERROR"
			)
			else
			(
				local l_directoriMesh = (get_FileName() + LevelList.items[LevelList.selection] +"\\Meshes\\")
				makeDir l_directoriMesh all: true
				file=fopen (l_directoriMesh+$.name+".mesh") "wb"
				writeMeshFromObject file ($)
				FClose file
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
)