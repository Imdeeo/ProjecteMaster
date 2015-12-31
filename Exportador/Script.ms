global isDebug = false
global UABVtxsNormals=#() 

fn get_LocalFileName =
(
	return "Game\\Data\\"
)

fn get_FileName =
(
	return "F:\\" + (get_LocalFileName())
)

fn get_Cores Objs =
(
	local cores = #()
	local areInstances
	append cores Objs[1]
	setUserProp Objs[1] "core_name" Objs[1].name
	for i = 2 to Objs.count do
	(
		areInstances = false
		for j = 1 to cores.count do
		(
			if ((AreNodesInstances Objs[i] cores[j]) == true) then
			(
				areInstances = true
				setUserProp Objs[i] "core_name" cores[j].name
			)
		)
		if areInstances == false then
		(
			append cores Objs[i]
			setUserProp Objs[i] "core_name" Objs[i].name
		)
	)
	return cores
)

fn dirExist dir =
(
	if (doesFileExist dir) == false then
		makeDir dir all:true
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

fn UABClearNormalsArray = 
(
	while UABVtxsNormals.count>0 do 
	(
		deleteItem UABVtxsNormals 1
	)
)

fn IsSmoothingGroupEnabled IdGroup Value = 
(
	local ValueMask=2^(IdGroup-1) 
	return (bit.and Value ValueMask)==ValueMask 
)

fn UABGetVertexNormal obj IdVertex SmoothValue FaceNormal = 
( 
	local HasNormal=false 
	local Normal=point3 0 0 0 
	for b=1 to 32 do 
	( 
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then 
		( 
			Normal=Normal+UABVtxsNormals[IdVertex][b] 
			HasNormal=true 
		) 
	) 
	if HasNormal==false then 
	( 
		Normal=FaceNormal 
	) 
	return Normal 
)

fn UABCalcVertexsNormals obj = 
( 
	UABClearNormalsArray() 
	local NumVtxs=getNumVerts obj 
	for b=1 to NumVtxs do 
	( 
		UABVtxsNormals[b]=#() 
		for t=1 to 32 do 
		( 
			UABVtxsNormals[b][t]=point3 0 0 0  
		) 
	) 
	local NumFaces=getNumFaces obj 
	local InvTransform=inverse obj.transform 
	for IdFace=1 to NumFaces do 
	( 
		local IdxsFace=getFace obj IdFace 
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform 
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform 
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform 
		local FaceNormal=getFaceNormal obj IdFace 
		local SmoothValue=getFaceSmoothGroup obj IdFace 
		for b=1 to 32 do 
		( 
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(                                                    
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal 
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal 
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + FaceNormal 
			) 
		) 
	) 
)

fn circumcenter p1 p2 p3 =
(
    BC = distance p2 p3
    CA = distance p3 p1
    AB = distance p1 p2

    baryCoords = [ (BC^2 * (CA^2 + AB^2 - BC^2)), (CA^2 * (AB^2 + BC^2 - CA^2)), (AB^2 * (BC^2 + CA^2 - AB^2)) ]
    triArea = baryCoords.x + baryCoords.y + baryCoords.z
    baryCoords /= triArea

    baryCoords.x * p1 + baryCoords.y * p2 + baryCoords.z * p3
)

fn getBoundingSphere Obj =
(
	struct vtxPair (vtxA, vtxB, dist)
	local vertexs = #()
	local vtxA
	local vtxB
	local vtxC

	for i = 1 to getNumVerts Obj do
	(
		append vertexs (getVert obj i)
	)

	local farVtxs = vtxPair dist:0
	for i = 1 to vertexs.count do
	(
		vtxA = vertexs[i]
	   
		for j = 1 to vertexs.count do
		(
			vtxB = vertexs[j]
		   
			if (distance vtxA vtxB) > farVtxs.dist do
			(
				farVtxs.vtxA = vtxA
				farVtxs.vtxB = vtxB
				farVtxs.dist = (distance vtxA vtxB)
			)
		)
	)

	center = ((farVtxs.vtxA + farVtxs.vtxB) / 2)
	radius = ((distance farVtxs.vtxA farVtxs.vtxB) / 2)

	local vtxsOut = #()
	for i = 1 to vertexs.count do
	(
		if ((distance vertexs[i] center) > radius) do append vtxsOut vertexs[i]
	)

	if vtxsOut.count != 0 then(
		for i = 1 to vtxsOut.count do
		(
			vtxA = vtxsOut[i]
			
			for j = 1 to vertexs.count do
			(
				vtxB = vertexs[j]
			   
				if ((distance vtxB vtxA) > (radius * sqrt 2)) do
				(
					for k = 1 to vertexs.count do
					(
						vtxC = vertexs[k]
					   
						if ((distance vtxC vtxA) > (radius * sqrt 2)) and ((distance vtxC vtxB) > (radius * sqrt 2)) do
						(
							c = (circumcenter vtxA vtxB vtxC)
							r = (distance c vtxA)
							if r > radius do
							(
								radius = r
								center = c
							)
						)
					)
				)
			)
		)
	)
	
	return #(center, radius)
)

fn createStaticMesh dir level filename Objs=
(	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			format "\t<static_mesh name=\"%\" filename=\"%\"/>\n" Objs[i].name ("Data\\" + level + "\\Meshes\\"+Objs[i].name+".mesh") to:file
		)
	)
	format "</static_meshes>\n" to:file
	close file
)

fn createInstanceMesh dir filename Objs=
(	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			Obj = Objs[i]
			print (getUserProp Obj "core_name")
			pos = TranslationToRH Obj.transform.translation
			format "\t<renderable_object name=\"%\" core_name=\"%\" position=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n" Obj.name (getUserProp Obj "core_name") pos[1] pos[2] pos[3] (GetYaw Obj.transform) (GetPitch Obj.transform) (GetRoll Obj.transform) true to:file
		)
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

fn writeMesh arx vertexs indexs BBMin BBMax BSphere=
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
	writeFloat arx BBMin.x
	writeFloat arx BBMin.y
	writeFloat arx BBMin.z
	writeFloat arx BBMax.x
	writeFloat arx BBMax.y
	writeFloat arx BBMax.z
	writeFloat arx BSphere[1].x
	writeFloat arx BSphere[1].y
	writeFloat arx BSphere[1].z
	writeFloat arx BSphere[2]
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
	
	l_Obj.transform *= inverse(l_Obj.transform)
	
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
		UABCalcVertexsNormals l_Obj
		local FaceNormal=getFaceNormal l_Obj i
		local SmoothValue=getFaceSmoothGroup l_Obj i
		local Vt1=getVert l_Obj IdxsFace.x
		local Vt2=getVert l_Obj IdxsFace.y
		local Vt3=getVert l_Obj IdxsFace.z
		local Nm1=normalize(UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal)
		local Nm2=normalize(UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal)
		local Nm3=normalize(UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal)
		/*local Nm1=getNormal l_Obj IdxsFace.x
		local Nm2=getNormal l_Obj IdxsFace.y
		local Nm3=getNormal l_Obj IdxsFace.z*/
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
	
	local ffd=ffdbox()
	addModifier l_Obj ffd
	BBMin=getModContextBBoxMin l_Obj ffd
	BBMax=getModContextBBoxMax l_Obj ffd
	
	BSphere = getBoundingSphere l_Obj
	
	writeMesh arx vertexs indexs BBMin BBMax BSphere
	
	if l_ObjectConverted==true then
	(
		delete l_Obj
	)
)

fn writeMeshFromObject arx Obj =
(
	writeHeader arx
	getMaterials arx Obj
	procesFaces arx Obj
	writeFooter arx
)

fn createMeshFromObjects dir Objs =
(
	dirExist (dir+"\\Meshes")
	for i=1 to Objs.count do
	(
		file=fopen (dir+"\\Meshes\\"+Objs[i].name+".mesh") "wb"
		writeMeshFromObject file (Objs[i])
		FClose file
	)
)

fn createMaterials dir filename Objs copyTexture =
(	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	if Objs != undefined then
	(
		materialsDictionary = dotNetObject "System.Collections.Hashtable"
		for i=1 to Objs.count do
		(
			mat = #()
			n_materials = 1
			
			if(classof(Objs[i].material)==Multimaterial)then
			(
				mat = Objs[i].material
				n_materials = Objs[i].material.numsubs
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
							
							pathfile = (dir+"Textures\\" + nomTextura)
							format "\t\t<texture filename=\"%\"/>\n" pathfile to:file
							if copyTexture == true then
							(
								dirExist (dir+"Textures")
								copyFile mat[j].diffuseMap.bitmap.filename pathfile
							)
						)
					)
					format "\t</material>\n" to:file 
				)
			)
		)
	)
	format "</materials>\n" to:file		
	close file
)

fn createLights dir filename Objs =
(
	local lightPos
	local lightDir
	local lightColor

	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<lights>\n" to:file
	
	for i=1 to Objs.count do
	(
		lightPos = Objs[i].pos.x as string+" "+Objs[i].pos.y as string+" "+Objs[i].pos.z as string
		lightColor = (Objs[i].color.r/255*100) as string+" "+(Objs[i].color.g/255*100) as string+" "+(Objs[i].color.b/255*100) as string
		
		if (ClassOf Objs[i] == Omnilight) then
		(
		format "\t<light name=\"%\" type=\"omni\" pos=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\">\n" Objs[i].name lightPos lightColor Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		) 
		else if (ClassOf Objs[i] == Directionallight or ClassOf Objs[i] == TargetDirectionallight) then
		(
			lightDir = Objs[i].dir.x as string+" "+Objs[i].dir.y as string+" "+Objs[i].dir.z as string
			format "\t<light name=\"%\" type=\"directional\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\">\n" Objs[i].name lightPos lightDir lightColor Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		) 
		else if (ClassOf Objs[i] == freeSpot or ClassOf Objs[i] == targetSpot) then
		(
			lightDir = Objs[i].dir.x as string+" "+Objs[i].dir.y as string+" "+Objs[i].dir.z as string
			format "\t<light name=\"%\" type=\"spot\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\">\n" Objs[i].name lightPos lightDir lightColor Objs[i].farAttenStart Objs[i].farAttenEnd Objs[i].multiplier to:file
		)
	)
	format "</lights>\n" to:file		
	close file
)

fn createCameras dir Objs =
(
	local cameraFileDir
	local cameraPos
	local cameraDir
	
	file=createfile (dir+"cameras.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<cameras>\n" to:file
	for i=1 to Objs.count do
	(
		select Objs[i]
		cameraPos = Objs[i].pos.x as string+" "+Objs[i].pos.y as string+" "+Objs[i].pos.z as string
		local firstframe = at time 999999999999 trackbar.getNextKeyTime()
		if (firstframe != undefined) and ((getUserProp Objs[i] "camera_export_type") != "static") then(
			dirExist (dir+"cameras\\")
			cameraFileDir = "cameras\\"+Objs[i].name+".xml"
			format "\t<camera name=\"%\" type=\"%\" pos=\"%\" dir=\"%\">\n" Objs[i].name (getUserProp Objs[i] "camera_export_type") cameraPos cameraFileDir to:file
			local j = firstframe
			cameraFile=createfile (dir+cameraFileDir)
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:cameraFile
			format "<camera_key_controller name=\"%\">\n" Objs[i].name to:cameraFile
			do (
				cameraPos = (at time j Objs[i].pos.x) as string+" "+(at time j Objs[i].pos.y) as string+" "+(at time j Objs[i].pos.z) as string
				cameraDir = (at time j Objs[i].dir.x) as string+" "+(at time j Objs[i].dir.y) as string+" "+(at time j Objs[i].dir.z) as string
				format "\t<key time=\"%\" pos=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\">\n" j cameraPos cameraDir (at time j $.fov) (at time j $.nearrange) (at time j $.farrange) to:cameraFile
				j = at time j trackbar.getNextKeyTime()
			) while j != firstframe
			format "</camera_key_controller>\n" to:cameraFile		
			close cameraFile
		) else (
			format "\t<camera name=\"%\" type=\"%\" pos=\"%\">\n" Objs[i].name (getUserProp Objs[i] "camera_export_type") cameraPos to:file
		)
		max select none
	)
	format "</cameras>\n" to:file		
	close file
)

/*fn createFiles Dir Objs =
(
	createInstanceMesh Dir "renderable_objects.xml" Objs
	createStaticMesh Dir "static_meshes.xml" Objs
	createMaterials Dir "materials.xml" Objs true
	createMeshFromObjects Dir Objs
	createLights Dir "lights.xml" Objs
	createCameras Dir Objs
)*/
--createFiles get_Dir() $*