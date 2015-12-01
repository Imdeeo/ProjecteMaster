global isDebug = false

fn get_LocalFileName =
(
	return "Data\\Level\\"
)

fn get_FileName =
(
	return "E:\\Game\\" + (get_LocalFileName())
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
	dirExist dir
	
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
	dirExist dir
	
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

fn createMeshFromObjects dir Objs =
(
	dirExist (Dir+"Meshes")
	
	for i=1 to Objs.count do
	(
		file=fopen (dir+"Meshes\\"+Objs[i].name+".mesh") "wb"
		writeMeshFromObject file (Objs[i])
		FClose file
	)
)

fn createMaterials dir filename Objs copyTexture =
(
	dirExist dir
	
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
							
							pathfile = (dir + "Textures\\" + nomTextura)
							format "\t\t<texture filename=\"%\"/>\n" pathfile to:file
							
							if copyTexture == true then
							(								
								if (doesFileExist (dir + "Textures")) == false then
								(									
									dirExist (dir+"Textures")
								)
								copyFile mat[j].diffuseMap.bitmap.filename pathfile
							)
						)
					)
					format "\t</material>\n" to:file 
				)
			)
	)		
	format "</materials>\n" to:file		
	close file
)

fn dirExist dir =
(
	if (doesFileExist dir) == false then
	(
		makeDir dir all:true
	)
)

fn createFiles Dir Objs =
(
	createInstanceMesh Dir "renderable_objects.xml" Objs
	createStaticMesh Dir "static_meshes.xml" Objs
	createMaterials Dir "materials.xml" Objs
	createMeshFromObjects Dir Objs
)

--createFiles (get_FileName()) $*