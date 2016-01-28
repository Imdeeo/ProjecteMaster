local aFaces = (sqrt nFaces) as integer
	nFaces -= (aFaces*aFaces)
	local i=1
	for a=1 to aFaces do
	(	
		for b=1 to aFaces do
		(
			local l_IdMaterial = (mod (getFaceMatID Obj i) l_TotalMaterials)+1
			local IdxsFace=getFace Obj i
			
			Vt1=getVert Obj IdxsFace.x
			Vt2=getVert Obj IdxsFace.y
			Vt3=getVert Obj IdxsFace.z
			
			local UABVtxsNormals = #()
			UABCalcVertexsNormals Obj &UABVtxsNormals
			
			local FaceNormal=getFaceNormal Obj i
			local SmoothValue=getFaceSmoothGroup Obj i
			
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
			i+=1
		)
	)
	for c=1 to nFaces do
	(
		local l_IdMaterial = (mod (getFaceMatID Obj i) l_TotalMaterials)+1
		local IdxsFace=getFace Obj i
		
		Vt1=getVert Obj IdxsFace.x
		Vt2=getVert Obj IdxsFace.y
		Vt3=getVert Obj IdxsFace.z
		
		local UABVtxsNormals = #()
		UABCalcVertexsNormals Obj &UABVtxsNormals
		
		local FaceNormal=getFaceNormal Obj i
		local SmoothValue=getFaceSmoothGroup Obj i
		
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
		i+=1
	)