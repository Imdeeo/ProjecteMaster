fn IsSmoothingGroupEnabled IdGroup Value &ret= 
(
	ret = ((bit.and Value (2^(IdGroup-1) ))==ValueMask)
)


fn UABGetVertexNormal obj IdVertex SmoothValue FaceNormal UABVtxsNormals &Normal = 
( 
	local HasNormal=false 
	local Normal=point3 0 0 0 
	local isSmooth
	for b=1 to 32 do 
	( 
		IsSmoothingGroupEnabled b SmoothValue &isSmooth
		
			Normal += UABVtxsNormals[IdVertex][b] 
			HasNormal=true 
		
	) 
	if HasNormal==false then 
	( 
		Normal=FaceNormal 
	) 
)

fn UABCalcVertexsNormals obj NumFaces &UABVtxsNormals= 
( 
	for IdFace=1 to NumFaces do 
	( 
		local IdxsFace=getFace obj IdFace 
		local FaceNormal=getFaceNormal obj IdFace 
		local SmoothValue=getFaceSmoothGroup obj IdFace 
		local isSmooth
		
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
	)
)