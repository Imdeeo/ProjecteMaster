local aFaces = (sqrt nFaces) as integer
nFaces -= (aFaces*aFaces)
local i=1
for a=1 to aFaces do
(	
	for b=1 to aFaces do
	(
		local l_IdMaterial = (mod (getFaceMatID l_Obj i) l_TotalMaterials)+1
		local IdxsFace=getFace l_Obj i
		local FaceNormal=getFaceNormal l_Obj i
		local SmoothValue=getFaceSmoothGroup l_Obj i
		local isSmooth
		
		Vt1=(getVert l_Obj IdxsFace.x)*InvTransform
		Vt2=(getVert l_Obj IdxsFace.y)*InvTransform
		Vt3=(getVert l_Obj IdxsFace.z)*InvTransform
		getBoundingSphere Vt1 &BSphere
        getBoundingSphere Vt2 &BSphere
        getBoundingSphere Vt3 &BSphere
		
		UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal UABVtxsNormals &Nm1
		UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal UABVtxsNormals &Nm2
		UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal UABVtxsNormals &Nm3
		Nm1=normalize(Nm1)
		Nm2=normalize(Nm2)
		Nm3=normalize(Nm3)
		
		if l_Obj.material != undefined then
		(
			
			if l_HasMultimaterial then
			(
				local l_DiffMap = l_Obj.material.materialList[l_IdMaterial].diffuseMap
				local l_LightMap = l_Obj.material.materialList[l_IdMaterial].selfIllumMap
			) else (
				local l_DiffMap = l_Obj.material.diffuseMap
				local l_LightMap = l_Obj.material.selfIllumMap
			)
			
			if classOf l_DiffMap == Bitmaptexture then
			(
				UVID = 0x0020
				local IdxsMap=meshop.getMapFace l_Obj 1 i
				TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap.x
				TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap.y
				TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap.z
			)
			
			if (classOf l_LightMap != undefined) and (meshop.getMapSupport l_Obj 2) then
			(
				UV2ID = 0x0040
				local IdxsMap=meshop.getMapFace l_Obj 2 i
				T2UVMap1= meshop.getMapVert l_Obj 2 IdxsMap.x
				T2UVMap2= meshop.getMapVert l_Obj 2 IdxsMap.y
				T2UVMap3= meshop.getMapVert l_Obj 2 IdxsMap.z
			)
		) else (
			ColorID = 0x0002
			defaultVCFaces l_Obj
			local ColorFace = getVCFace l_Obj i
			Color1 = getVertColor l_Obj ColorFace.x
			Color2 = getVertColor l_Obj ColorFace.y
			Color3 = getVertColor l_Obj ColorFace.z
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
		prog.value = 100.*i/nFaces
		i+=1
	)
)
for c=1 to nFaces do
(
	local l_IdMaterial = (mod (getFaceMatID l_Obj i) l_TotalMaterials)+1
	local IdxsFace=getFace l_Obj i
	local FaceNormal=getFaceNormal l_Obj i
	local SmoothValue=getFaceSmoothGroup l_Obj i
	local isSmooth
	
	Vt1=(getVert l_Obj IdxsFace.x)*InvTransform
	Vt2=(getVert l_Obj IdxsFace.y)*InvTransform
	Vt3=(getVert l_Obj IdxsFace.z)*InvTransform
	getBoundingSphere Vt1 &BSphere
	getBoundingSphere Vt2 &BSphere
	getBoundingSphere Vt3 &BSphere
	
	UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal UABVtxsNormals &Nm1
	UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal UABVtxsNormals &Nm2
	UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal UABVtxsNormals &Nm3
	Nm1=normalize(Nm1)
	Nm2=normalize(Nm2)
	Nm3=normalize(Nm3)
	
	if l_Obj.material != undefined then
	(
		
		if l_HasMultimaterial then
		(
			local l_DiffMap = l_Obj.material.materialList[l_IdMaterial].diffuseMap
			local l_LightMap = l_Obj.material.materialList[l_IdMaterial].selfIllumMap
		) else (
			local l_DiffMap = l_Obj.material.diffuseMap
			local l_LightMap = l_Obj.material.selfIllumMap
		)
		
		if classOf l_DiffMap == Bitmaptexture then
		(
			UVID = 0x0020
			local IdxsMap=meshop.getMapFace l_Obj 1 i
			TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap.x
			TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap.y
			TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap.z
		)
		
		if (classOf l_LightMap != undefined) and (meshop.getMapSupport l_Obj 2) then
		(
			UV2ID = 0x0040
			local IdxsMap=meshop.getMapFace l_Obj 2 i
			T2UVMap1= meshop.getMapVert l_Obj 2 IdxsMap.x
			T2UVMap2= meshop.getMapVert l_Obj 2 IdxsMap.y
			T2UVMap3= meshop.getMapVert l_Obj 2 IdxsMap.z
		)
	) else (
		ColorID = 0x0002
		defaultVCFaces l_Obj
		local ColorFace = getVCFace l_Obj i
		Color1 = getVertColor l_Obj ColorFace.x
		Color2 = getVertColor l_Obj ColorFace.y
		Color3 = getVertColor l_Obj ColorFace.z
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
	prog.value = 100.*i/nFaces
	i+=1
)


for i=1 to nFaces do
(	
	local l_IdMaterial = (mod (getFaceMatID l_Obj i) l_TotalMaterials)+1
	local IdxsFace=getFace l_Obj i
	local FaceNormal=getFaceNormal l_Obj i
	local SmoothValue=getFaceSmoothGroup l_Obj i
	local isSmooth
	
	Vt1=(getVert l_Obj IdxsFace.x)*InvTransform
	Vt2=(getVert l_Obj IdxsFace.y)*InvTransform
	Vt3=(getVert l_Obj IdxsFace.z)*InvTransform
	getBoundingSphere Vt1 &BSphere
	getBoundingSphere Vt2 &BSphere
	getBoundingSphere Vt3 &BSphere
	
	UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal UABVtxsNormals &Nm1
	UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal UABVtxsNormals &Nm2
	UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal UABVtxsNormals &Nm3
	Nm1=normalize(Nm1)
	Nm2=normalize(Nm2)
	Nm3=normalize(Nm3)
	
	if l_Obj.material != undefined then
	(
		
		if l_HasMultimaterial then
		(
			local l_DiffMap = l_Obj.material.materialList[l_IdMaterial].diffuseMap
			local l_LightMap = l_Obj.material.materialList[l_IdMaterial].selfIllumMap
		) else (
			local l_DiffMap = l_Obj.material.diffuseMap
			local l_LightMap = l_Obj.material.selfIllumMap
		)
		
		if classOf l_DiffMap == Bitmaptexture then
		(
			UVID = 0x0020
			local IdxsMap=meshop.getMapFace l_Obj 1 i
			TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap.x
			TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap.y
			TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap.z
		)
		
		if (classOf l_LightMap != undefined) and (meshop.getMapSupport l_Obj 2) then
		(
			UV2ID = 0x0040
			local IdxsMap=meshop.getMapFace l_Obj 2 i
			T2UVMap1= meshop.getMapVert l_Obj 2 IdxsMap.x
			T2UVMap2= meshop.getMapVert l_Obj 2 IdxsMap.y
			T2UVMap3= meshop.getMapVert l_Obj 2 IdxsMap.z
		)
	) else (
		ColorID = 0x0002
		defaultVCFaces l_Obj
		local ColorFace = getVCFace l_Obj i
		Color1 = getVertColor l_Obj ColorFace.x
		Color2 = getVertColor l_Obj ColorFace.y
		Color3 = getVertColor l_Obj ColorFace.z
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
	prog.value = 100.*i/nFaces
)