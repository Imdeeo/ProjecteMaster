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
	
	Obj.transform *= inverse(Obj.transform)
	
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
		
		Vt1=getVert Obj IdxsFace.x
		Vt2=getVert Obj IdxsFace.y
		Vt3=getVert Obj IdxsFace.z
		
		UABCalcVertexsNormals Obj
		local FaceNormal=getFaceNormal Obj i
		local SmoothValue=getFaceSmoothGroup Obj i
		Nm1=normalize(UABGetVertexNormal Obj IdxsFace.x SmoothValue FaceNormal)
		Nm2=normalize(UABGetVertexNormal Obj IdxsFace.y SmoothValue FaceNormal)
		Nm3=normalize(UABGetVertexNormal Obj IdxsFace.z SmoothValue FaceNormal)
		
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
		
		local v1 = createVertex Vt1 Color1 Nm1 TUVMap1 T2UVMap1
		local v2 = createVertex Vt2 Color2 Nm2 TUVMap2 T2UVMap2
		local v3 = createVertex Vt3 Color3 Nm3 TUVMap3 T2UVMap3
		
		local id1=getIndex vertexs v1 l_IdMaterial vertexsDictionary
		local id2=getIndex vertexs v2 l_IdMaterial vertexsDictionary
		local id3=getIndex vertexs v3 l_IdMaterial vertexsDictionary
		
		append indexs[l_IdMaterial] id1
		append indexs[l_IdMaterial] id2
		append indexs[l_IdMaterial] id3
	)
	
	local ffd=ffdbox()
	addModifier Obj ffd
	BBMin=getModContextBBoxMin Obj ffd
	BBMax=getModContextBBoxMax Obj ffd
	BSphere = getBoundingSphere Obj	
	l_VtxID = VtID+ColorID+NmID+UVID+UV2ID
	
	writeMesh arx l_VtxID vertexs indexs BBMin BBMax BSphere
)

fn writeMeshFromObject arx Obj =
(
	WriteShort arx 0xFE55
	getMaterials arx Obj
	processFaces arx Obj
	WriteShort arx 0x55FE
)

fn createMeshFromObjects dir Objs =
(
	dirExist (dir+"\\Meshes")
	for i=1 to Objs.count do
	(
		if Objs[i].material != undefined then
		(
			file=fopen (dir+"\\Meshes\\"+Objs[i].name+".mesh") "wb"
			writeMeshFromObject file (Objs[i])
			FClose file
		) else (
			format "The object % has no material!\n" Objs[i].name
		)
	)
)
