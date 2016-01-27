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

fn mat3xvect3 mat vect =
( 
    local res = [0, 0, 0]
	local x = vect.x
	local y = vect.y
	local z = vect.z
    res.x = (mat.row1.x*x) + (mat.row1.y*y) + (mat.row1.z*z)
    res.y = (mat.row2.x*x) + (mat.row2.y*y) + (mat.row2.z*z)
    res.z = (mat.row3.x*x) + (mat.row3.y*y) + (mat.row3.z*z)
    return res
)

fn mat4xvect3 mat vect w =
( 
    -- w=1 -> Translate // w=0 -> Rotate
    local res = [0, 0, 0]
	local x = vect.x
	local y = vect.y
	local z = vect.z
    res.x = (mat.row1.x*x) + (mat.row2.x*y) + (mat.row3.x*z) + (mat.row4.x*w)
    res.y = (mat.row1.y*x) + (mat.row2.y*y) + (mat.row3.y*z) + (mat.row4.y*w)
    res.z = (mat.row1.z*x) + (mat.row2.z*y) + (mat.row3.z*z) + (mat.row4.z*w)
    return res
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

fn createVertex pos col norm textcoord1 textcoord2 =
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
	if(textcoord1!=undefined)then
	(
		append vertex textcoord1.x
		append vertex textcoord1.y
	)
	if(textcoord2!=undefined)then
	(
		append vertex textcoord2.x
		append vertex textcoord2.y
	)
	return vertex
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

fn getIndex vertexVect vertex IdMaterial vertexsDictionary =
(
	local vtxStr = vertex as string
	local vtxN
	local doContain = vertexsDictionary.ContainsKey vtxStr

	if doContain then
	(
		vtxN = vertexsDictionary.Item[vtxStr]
	) else (
		vtxN = vertexVect[IdMaterial].count
		vertexsDictionary.Add vtxStr vtxN
		append vertexVect[IdMaterial] vertex
	)
	
	return vtxN
	/*
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
	*/
)
