fileIn "Math.ms"

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

fn TransformToRH matrix =
(
    local r1 = matrix.row1
    local r2 = matrix.row2
    local r3 = matrix.row3
    local r4 = TranslationToRH matrix.row4
    return matrix3 r1 r2 r3 r4
)

fn GetYaw trans =
(
	return (-(trans as eulerangles).z*PI/180)
)

fn GetPitch trans =
(
	return ((-(trans as eulerangles).y*PI/180)+(PI/2))
)

fn GetRoll trans =
(
	return ((trans as eulerangles).x*PI/180)
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


fn createCinematicObjects dir filename Objs=
(	
	file=createfile (dir+filename)
	
	local max_time=0
	for i=1 to Objs.count do
	(
		select Objs[i]
		if (trackbar.getPreviousKeyTime()!=undefined) then
		(
			local obj_time = (at time 999999999999 trackbar.getPreviousKeyTime())
			if obj_time > max_time then
			(
				max_time = obj_time
			)	
		)
	)
	
	local key
	key = max_time as string
	key = replace key key.count 1 ""
	key = (key as float)/30
		
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<cinematic name=\"%\" duration=\"%\">\n" maxFileName key to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			select Objs[i]
			
			if (trackbar.getPreviousKeyTime()!=undefined) then
			(
				Obj = Objs[i]
				format "\t<cinematic_object resource=\"%\" renderable_objects_manager=\"%\">\n" Obj.name (getUserProp Obj "render_layer") to:file			
				
				local firstframe = at time 999999999999 trackbar.getNextKeyTime()
				local j = firstframe
				do (
					key = j as string	
					key = replace key key.count 1 ""
					key = (key as float)/30

					pos = TranslationToRH Obj.transform.translation				
					scl = TranslationToRH Obj.transform.scale
						
					format "\t\t<cinematic_object_key_frame time=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" scale=\"% % %\"/>\n" key pos[1] pos[2] pos[3] (GetYaw Obj.transform) (GetPitch Obj.transform) (GetRoll Obj.transform) scl[1] scl[2] scl[3] to:file
						
					j = at time j trackbar.getNextKeyTime()
				) while j != firstframe
				
				format "\t</cinematic_object>\n" to:file			
			)
		)
	)
	format "</cinematic>\n" to:file
	close file
	
	/*//////ESTO NO FUNCIONAAAAA	
		cameraTransform = Objs[i].transform
		cameraTranslation = TranslationToRH (cameraTransform.translation)
		cameraPos = cameraTranslation.x as string+" "+cameraTranslation.y as string+" "+cameraTranslation.z as string
		local firstframe = at time 999999999999 trackbar.getNextKeyTime()
		key = (at time 999999999999 trackbar.getPreviousKeyTime()) as string
		key = replace key key.count 1 ""
		key = (key as float)/30
		if (firstframe != undefined) and ((getUserProp Objs[i] "camera_export_type") != "static") then(
			dirExist (dir+"cameras\\")
			cameraFileDir = "cameras\\"+Objs[i].name+".xml"
			cameraType = getUserProp Objs[i] "camera_export_type"
			format "\t<camera name=\"%\" total_time=\"%\" type=\"key\" %=\"true\" pos=\"%\" filename=\"%\"/>\n" Objs[i].name key cameraType cameraPos cameraFileDir to:file
			local j = firstframe
			cameraFile=createfile (dir+cameraFileDir)
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:cameraFile
			format "<camera_key_controller name=\"%\">\n" Objs[i].name to:cameraFile
			do (
				key = j as string	
				key = replace key key.count 1 ""
				key = (key as float)/30
				cameraTransform = at time j Objs[i].transform
				cameraTranslation = TranslationToRH (cameraTransform.translation)
				cameraPos = cameraTranslation.x as string+" "+cameraTranslation.y as string+" "+cameraTranslation.z as string
				cameraUp = mat3xvect3 cameraTransform z_axis
				cameraUp = (CameraUp.x) as string+" "+(CameraUp.y) as string+" "+(CameraUp.z) as string
				cameraLookAt = (cameraTranslation + (mat3xvect3 cameraTransform y_axis))
				cameraLookAt = (cameraLookAt.x) as string+" "+(cameraLookAt.y) as string+" "+(cameraLookAt.z) as string
				format "\t<key time=\"%\" pos=\"%\" up=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraPos cameraUp cameraLookAt (at time j Objs[i].fov) (at time j Objs[i].nearrange) (at time j Objs[i].farrange) to:cameraFile
				j = at time j trackbar.getNextKeyTime()
			) while j != firstframe
			format "</camera_key_controller>\n" to:cameraFile		
			close cameraFile*/
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
	if (Obj.material!=undefined)then
	(
		if(classof(Obj.material)==Multimaterial)then
		(
			if isDebug then
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
			writeLong arx 1
			WriteShort arx Obj.material.name.count #unsigned
			writeString arx Obj.material.name
		)
	)
	else
	(
		n_materials = 0
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

fn writeMesh arx VtxID vertexs indexs BBMin BBMax BSphere=
(
	if isDebug then
	format "Number of materials: %\n" vertexs.count
	for i=1 to vertexs.count do
	(
		if isDebug then
		format "Material %:\n" i
		writeShort arx VtxID #unsigned
		if isDebug then
		format "Size of vertex: %\n" VtxID
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

fn createVertex pos col norm textcoord =
(
	local vertex = #()
	if(pos!=undefined) then
	(
		append vertex pos.x
		append vertex pos.y
		append vertex pos.z
	)
	if(col!=undefined) then
	(
		append vertex col.r
		append vertex col.g
		append vertex col.b
	)
	if(norm!=undefined) then
	(
		append vertex norm.x
		append vertex norm.y
		append vertex norm.z
	)
	if(textcoord!=undefined)then
	(
		append vertex textcoord.x
		append vertex textcoord.y
	)
	return vertex
)

fn procesFaces arx Obj =
(
	local l_VtxID = 0x0001
	local l_Obj=Obj
	local l_ObjectConverted=false
	
	local Vt1 = undefined
	local Vt2 = undefined
	local Vt3 = undefined
	local Color1 = undefined
	local Color2 = undefined
	local Color3 = undefined
	local Nm1 = undefined
	local Nm2 = undefined
	local Nm3 = undefined
	local TUVMap1 = undefined
	local TUVMap2 = undefined
	local TUVMap3 = undefined
	
	if classof Obj != Editable_mesh then
	(
		l_Obj=copy Obj
		convertToMesh l_Obj
		l_ObjectConverted=true
	)
	
	l_Obj.transform *= inverse(l_Obj.transform)
	
	local nFaces = getNumFaces l_Obj
	local vertexs = #()
	local indexs = #()
	
	if l_Obj.material != undefined then
	(	
		local l_HasMultimaterial=classof l_Obj.material==Multimaterial
		local l_TotalMaterials=1
		if l_HasMultimaterial then
			l_TotalMaterials=l_Obj.material.count
		
		for i=1 to l_TotalMaterials do
		(
			append vertexs #()
			append indexs #()
		)		
	) else (
		append vertexs #()
		append indexs #()
	)

	for i=1 to nFaces do
	(
		local l_IdMaterial
		local IdxsFace=getFace l_Obj i
		
		--Position
		Vt1=getVert l_Obj IdxsFace.x
		Vt2=getVert l_Obj IdxsFace.y
		Vt3=getVert l_Obj IdxsFace.z
		
		--Normal
		l_VtxID += 0x0004
		UABCalcVertexsNormals l_Obj
		local FaceNormal=getFaceNormal l_Obj i
		local SmoothValue=getFaceSmoothGroup l_Obj i
		Nm1=normalize(UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal)
		Nm2=normalize(UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal)
		Nm3=normalize(UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal)
		
		--Texture and material
		if l_Obj.material != undefined then
		(
			if l_Obj.material.diffuseMap != undefined or l_Obj.material.bumpMap != undefined or l_Obj.material.selfIllumMap != undefined
			(
				--Texture1
				l_VtxID += 0x0020
				local IdxsMap=meshop.getMapFace l_Obj 1 i
				TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap.x
				TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap.y
				TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap.z
				
				l_IdMaterial=getFaceMatID l_Obj i
				if l_HasMultimaterial==false then l_IdMaterial=1
			) else (
				--Color
				l_VtxID += 0x0002
				defaultVCFaces l_Obj
				local ColorFace = getVCFace l_Obj i
				Color1 = getVertColor l_Obj ColorFace.x
				Color2 = getVertColor l_Obj Colorface.y
				Color3 = getVertColor l_Obj ColorFace.z
				l_IdMaterial = 1
			)
		) else (
			--Color
			l_VtxID += 0x0002
			local l_IdMaterial = 1
			defaultVCFaces l_Obj
			local ColorFace = getVCFace l_Obj i
			Color1 = getVertColor l_Obj ColorFace.x
			Color2 = getVertColor l_Obj Colorface.y
			Color3 = getVertColor l_Obj ColorFace.z
		)
		
		--Vertex creation
		local v1 = createVertex Vt1 Color1 Nm1 TUVMap1
		local v2 = createVertex Vt2 Color2 Nm2 TUVMap2
		local v3 = createVertex Vt3 Color3 Nm3 TUVMap3
		
		-- Index creation
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
	
	--Bounding box
	local ffd=ffdbox()
	addModifier l_Obj ffd
	BBMin=getModContextBBoxMin l_Obj ffd
	BBMax=getModContextBBoxMax l_Obj ffd
	
	--Bounding sphere
	BSphere = getBoundingSphere l_Obj	
	
	if isDebug then format "Vertex ID: %" l_VtxID
	
	writeMesh arx l_VtxID vertexs indexs BBMin BBMax BSphere
	
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
			
			if (Objs[i].material!=undefined) then
			(
				if(classof(Objs[i].material)==Multimaterial)then
				(
					format "Multimaterial\n"
					mat = Objs[i].material
					n_materials = Objs[i].material.numsubs
				)
				else
				(
					format "Un material\n"
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
	local cameraTransform
	local cameraTranslation
	local cameraPos
	local cameraUp
	local cameraLookAt
	local key
	local cameraType
	
	file=createfile (dir+"cameras.xml")
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<cameras>\n" to:file
	for i=1 to Objs.count do
	(
		select Objs[i]
		cameraTransform = Objs[i].transform
		cameraTranslation = TranslationToRH (cameraTransform.translation)
		cameraPos = cameraTranslation.x as string+" "+cameraTranslation.y as string+" "+cameraTranslation.z as string
		if (firstframe != undefined) and ((getUserProp Objs[i] "camera_export_type") != "static") then(
			local firstframe = at time 999999999999 trackbar.getNextKeyTime()
			key = (at time 999999999999 trackbar.getPreviousKeyTime()) as string
			key = replace key key.count 1 ""
			key = (key as float)/30		
			dirExist (dir+"cameras\\")
			cameraFileDir = "cameras\\"+Objs[i].name+".xml"
			cameraType = getUserProp Objs[i] "camera_export_type"
			format "\t<camera name=\"%\" total_time=\"%\" type=\"key\" %=\"true\" pos=\"%\" filename=\"%\"/>\n" Objs[i].name key cameraType cameraPos cameraFileDir to:file
			local j = firstframe
			cameraFile=createfile (dir+cameraFileDir)
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:cameraFile
			format "<camera_key_controller name=\"%\">\n" Objs[i].name to:cameraFile
			do (
				key = j as string	
				key = replace key key.count 1 ""
				key = (key as float)/30
				cameraTransform = at time j Objs[i].transform
				cameraTranslation = TranslationToRH (cameraTransform.translation)
				cameraPos = cameraTranslation.x as string+" "+cameraTranslation.y as string+" "+cameraTranslation.z as string
				cameraUp = mat3xvect3 cameraTransform z_axis
				cameraUp = (CameraUp.x) as string+" "+(CameraUp.y) as string+" "+(CameraUp.z) as string
				cameraLookAt = (cameraTranslation + (mat3xvect3 cameraTransform y_axis))
				cameraLookAt = (cameraLookAt.x) as string+" "+(cameraLookAt.y) as string+" "+(cameraLookAt.z) as string
				format "\t<key time=\"%\" pos=\"%\" up=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" key cameraPos cameraUp cameraLookAt (at time j Objs[i].fov) (at time j Objs[i].nearrange) (at time j Objs[i].farrange) to:cameraFile
				j = at time j trackbar.getNextKeyTime()
			) while j != firstframe
			format "</camera_key_controller>\n" to:cameraFile		
			close cameraFile
		) else (
			cameraUp = mat3xvect3 cameraTransform z_axis
			cameraUp = (CameraUp.x) as string+" "+(CameraUp.y) as string+" "+(CameraUp.z) as string
			if(Objs[i].target != undefined) then (cameraLookAt = Objs[i].target.pos)
			else (cameraLookAt = (cameraTranslation + (mat3xvect3 cameraTransform y_axis)))
			cameraLookAt = (cameraLookAt.x) as string+" "+(cameraLookAt.y) as string+" "+(cameraLookAt.z) as string
			cameraType = getUserProp Objs[i] "camera_export_type"
			format "\t<camera name=\"%\" type=\"%\" pos=\"%\" up=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" Objs[i].name cameraType cameraPos cameraUp cameraLookAt Objs[i].fov Objs[i].nearrange Objs[i].farrange to:file
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