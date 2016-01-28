fileIn "Math.ms"
fileIn "Utils.ms"
fileIn "UABVertexNormals.ms"
fileIn "material.ms"
global isDebug = false

fn writeMesh arx VtxID vertexs indexs BBMin BBMax BSphere=
(
	for i=1 to vertexs.count do
	(
		writeShort arx VtxID #unsigned
		writeLong arx vertexs[i].count
		for j=1 to vertexs[i].count do
		(
			for k=1 to vertexs[i][j].count do
			(
				writeFloat arx vertexs[i][j][k]
			)
		)
		writeShort arx 16 #unsigned
		writeLong arx indexs[i].count
		for j=1 to indexs[i].count do
		(
			writeShort arx indexs[i][j] #unsigned
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

fn processFaces arx Obj =
(	
	if classof Obj != Editable_mesh then
	(
		convertToMesh Obj
	)
	
	Obj.transform *= inverse(Obj.transform)
	local InvTransform=inverse Obj.transform
	
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
	local T2UVMap1 = undefined
	local T2UVMap2 = undefined
	local T2UVMap3 = undefined
	
	local VtID = 0x0001
	local ColorID = 0x0000
	local NmID = 0x0004
	local UVID = 0x0000
	local UV2ID = 0x0000
	
	local NumVtxs=getNumVerts Obj
	local UABVtxsNormals = #()
	for b=1 to NumVtxs do 
	( 
		UABVtxsNormals[b]=#() 
		for t=1 to 32 do 
		( 
			UABVtxsNormals[b][t]=point3 0 0 0  
		) 
	)
	
	local nFaces = getNumFaces Obj
	local vertexs = #()
	local indexs = #()
	local vertexsDictionary = dotnetobject "System.Collections.Generic.Dictionary`2[System.String, System.UInt32]"
	
	if Obj.material != undefined then
	(	
		local l_HasMultimaterial = classof Obj.material==Multimaterial
		local l_TotalMaterials = 1
		if l_HasMultimaterial then
			l_TotalMaterials=Obj.material.count
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
		
		local l_IdMaterial = (mod (getFaceMatID Obj i) l_TotalMaterials)+1
		local IdxsFace=getFace Obj i
		local FaceNormal=getFaceNormal Obj i
		local SmoothValue=getFaceSmoothGroup Obj i
		local isSmooth
		
		Vt1=getVert Obj IdxsFace.x
		Vt2=getVert Obj IdxsFace.y
		Vt3=getVert Obj IdxsFace.z
		
		for b=1 to 32 do
		( 
			IsSmoothingGroupEnabled b SmoothValue &isSmooth
			if(isSmooth==true) then
			(                                                    
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal 
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal 
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + FaceNormal 
			) 
		)
		
		UABGetVertexNormal Obj IdxsFace.x SmoothValue FaceNormal UABVtxsNormals &Nm1
		UABGetVertexNormal Obj IdxsFace.y SmoothValue FaceNormal UABVtxsNormals &Nm2
		UABGetVertexNormal Obj IdxsFace.z SmoothValue FaceNormal UABVtxsNormals &Nm3
		Nm1=normalize(Nm1)
		Nm2=normalize(Nm2)
		Nm3=normalize(Nm3)
		
		if Obj.material != undefined then
		(
			
			if l_HasMultimaterial then
			(
				local l_DiffMap = Obj.material.materialList[l_IdMaterial].diffuseMap
				local l_LightMap = Obj.material.materialList[l_IdMaterial].selfIllumMap
			) else (
				local l_DiffMap = Obj.material.diffuseMap
				local l_LightMap = Obj.material.selfIllumMap
			)
			
			if classOf l_DiffMap == Bitmaptexture then
			(
				UVID = 0x0020
				local IdxsMap=meshop.getMapFace Obj 1 i
				TUVMap1= meshop.getMapVert Obj 1 IdxsMap.x
				TUVMap2= meshop.getMapVert Obj 1 IdxsMap.y
				TUVMap3= meshop.getMapVert Obj 1 IdxsMap.z
			) else (
				ColorID = 0x0002
				defaultVCFaces Obj
				local ColorFace = getVCFace Obj i
				Color1 = getVertColor Obj ColorFace.x
				Color2 = getVertColor Obj Colorface.y
				Color3 = getVertColor Obj ColorFace.z
			)
			if (classOf l_LightMap != undefined) and (meshop.getMapSupport Obj 2) then
			(
				UV2ID = 0x0040
				local IdxsMap=meshop.getMapFace Obj 2 i
				T2UVMap1= meshop.getMapVert Obj 2 IdxsMap.x
				T2UVMap2= meshop.getMapVert Obj 2 IdxsMap.y
				T2UVMap3= meshop.getMapVert Obj 2 IdxsMap.z
			)
		) else (
			ColorID = 0x0002
			defaultVCFaces Obj
			local ColorFace = getVCFace Obj i
			Color1 = getVertColor Obj ColorFace.x
			Color2 = getVertColor Obj ColorFace.y
			Color3 = getVertColor Obj ColorFace.z
		)
		
		local v1 = #()
		local v2 = #()
		local v3 = #()
		createVertex Vt1 Color1 Nm1 TUVMap1 T2UVMap1 &v1
		createVertex Vt2 Color2 Nm2 TUVMap2 T2UVMap2 &v2
		createVertex Vt3 Color3 Nm3 TUVMap3 T2UVMap3 &v3
		
		local id1
		local id2
		local id3
		getIndex vertexs v1 l_IdMaterial vertexsDictionary &id1
		getIndex vertexs v2 l_IdMaterial vertexsDictionary &id2
		getIndex vertexs v3 l_IdMaterial vertexsDictionary &id3
		
		append indexs[l_IdMaterial] id1
		append indexs[l_IdMaterial] id2
		append indexs[l_IdMaterial] id3
		
		free UABVtxsNormals
		
	)
	
	local ffd=ffdbox()
	addModifier Obj ffd
	BBMin=getModContextBBoxMin Obj ffd
	BBMax=getModContextBBoxMax Obj ffd
	BSphere = #()
	getBoundingSphere Obj	&BSphere
	l_VtxID = VtID+ColorID+NmID+UVID+UV2ID
	
	writeMesh arx l_VtxID vertexs indexs BBMin BBMax BSphere	
)

fn writeMeshFromObject arx Obj =
(
	WriteShort arx 0xFE55
	local nMaterials
	getMaterials arx Obj
	processFaces arx Obj
	WriteShort arx 0x55FE
)

fn createMeshFromObjects dir Objs =
(
	dirExist (dir+"\\Meshes")
			
	for l_Obj in Objs do
	(
		if l_Obj.material != undefined then
		(
			file=fopen (dir+"\\Meshes\\"+l_Obj.name+".mesh") "wb"
			writeMeshFromObject file (l_Obj)
			FClose file
		) else (
			format "The object % has no material!\n" l_Obj.name
		)
	)
)
