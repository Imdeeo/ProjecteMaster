fn checkZeroes &ret =
(
	if ret.x < 0.0001 and ret.x > -0.0001 then( ret.x = 0 )
	if ret.y < 0.0001 and ret.y > -0.0001 then( ret.y = 0 )
	if ret.z < 0.0001 and ret.z > -0.0001 then( ret.z = 0 )
)

fn deg2rad &ret =
(
	ret = ret*PI/180
)

fn verticalizeFov &ret =
(
	ret *= 0.66931440580841338505245880369832
)

fn lerp minVal maxVal term &ret = 
(
    ret = (maxVal - minVal) * term + minVal
)

fn mat3xvect3 mat vect &res =
( 
	res = [0, 0, 0]
    res.x = (mat.row1.x*vect.x) + (mat.row1.y*vect.y) + (mat.row1.z*vect.z)
    res.y = (mat.row2.x*vect.x) + (mat.row2.y*vect.y) + (mat.row2.z*vect.z)
    res.z = (mat.row3.x*vect.x) + (mat.row3.y*vect.y) + (mat.row3.z*vect.z)
)

fn mat3xvect3Alternate mat vect &res =
( 
	res = [0, 0, 0]
    res.x = (mat.row1.x*vect.x) + (mat.row2.x*vect.y) + (mat.row3.x*vect.z)
    res.y = (mat.row1.y*vect.x) + (mat.row2.y*vect.y) + (mat.row3.y*vect.z)
    res.z = (mat.row1.z*vect.x) + (mat.row2.z*vect.y) + (mat.row3.z*vect.z)
)

fn mat4xvect3 mat vect w &res =
( 
    -- w=1 -> Translate // w=0 -> Rotate
    res = [0, 0, 0]
    res.x = (mat.row1.x*vect.x) + (mat.row2.x*vect.y) + (mat.row3.x*vect.z) + (mat.row4.x*vect.w)
    res.y = (mat.row1.y*vect.x) + (mat.row2.y*vect.y) + (mat.row3.y*vect.z) + (mat.row4.y*vect.w)
    res.z = (mat.row1.z*vect.x) + (mat.row2.z*vect.y) + (mat.row3.z*vect.z) + (mat.row4.z*vect.w)
)

fn negateXZ &vect =
(
	vect.x = -vect.x
	vect.z = -vect.z
)

fn swapYZ &vect =
(
	local aux = vect.y
	vect.y = vect.z
	vect.z = aux
)

fn swapXZ &vect =
(
	local aux = vect.x
	vect.x = vect.z
	vect.z = aux
)

fn swapXY &vect =
(
	local aux = vect.x
	vect.x = vect.y
	vect.y = aux
)

fn QuatToRH &q =
(
	q = [q.x, q.z, -q.y, q.w]
)

fn CustomQuatToRH &q =
(
	q = [q.x,q.z, -q.y, -q.w]
)

fn InverseQuat &q =
(
	q.w = -q.w
)

fn CustomToRH &translation =
(
	translation = [-translation.y, translation.z, translation.x]
)

fn AnimatedCameraToRH &translation =
(
	translation = [translation.x, translation.z, -translation.y]
)

fn TranslationToRH &translation =
(
	translation = [-translation.x, translation.z, translation.y]
)

fn TransformToRH &matrix =
(
    matrix.row4 = TranslationToRH matrix.row4
)

fn GetYaw trans &yaw =
(
	yaw = ((-(trans as eulerangles).z*PI/180))
)

fn GetPitch trans &pitch =
(
	pitch = ((-(trans as eulerangles).y*PI/180))
)

fn GetRoll trans &roll =
(
	roll = ((trans as eulerangles).x*PI/180)
)

fn getBoundingSphere vertex &BSphere =
(
	local dist = distance BSphere[1] vertex
	if dist > BSphere[2] then BSphere[2] = dist
)

fn getBoundingBox vertex &BBMax &BBMin =
(
	if vertex.x > BBMax.x then ( BBMax.x = vertex.x )
	if vertex.y > BBMax.y then ( BBMax.y = vertex.y )
	if vertex.z > BBMax.z then ( BBMax.z = vertex.z )
	if vertex.x < BBMin.x then ( BBMin.x = vertex.x )
	if vertex.y < BBMin.y then ( BBMin.y = vertex.y )
	if vertex.z < BBMin.z then ( BBMin.z = vertex.z )			
)

fn createVertex pos col norm textcoord1 textcoord2 binormal tangent &vertex =
(
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
		append vertex 1
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
		append vertex (1 - textcoord1.y)
	)
	if(textcoord2!=undefined)then
	(
		append vertex textcoord2.x
		append vertex (1 - textcoord2.y)
	)
	if(binormal==0x0100)then
	(
		append vertex 0
		append vertex 0
		append vertex 0
		append vertex 0
	)
	if(tangent==0x0200)then
	(
		append vertex 0
		append vertex 0
		append vertex 0
		append vertex 0
	)
)

fn getIndex vertexVect vertex IdMaterial vertexsDictionary &indice =
(
	local vtxStr = vertex as string
	local doContain = vertexsDictionary[IdMaterial].ContainsKey vtxStr

    if doContain then
	(
		indice = vertexsDictionary[IdMaterial].Item[vtxStr]
	) else (
		indice = 0
		for vect in vertexVect do
		(
			indice = indice + vect.count
		)
		indice = vertexVect[IdMaterial].count
		vertexsDictionary[IdMaterial].Add vtxStr indice
		append vertexVect[IdMaterial] vertex
	)
)

fn stahp uv =
(
	if (uv.x > 1 or uv.x < 0) then
	( 
		uv.x -= floor(uv.x)
	)
	if (uv.y > 1 or uv.y < 0) then
	( 
		uv.y -= floor(uv.y)
	)
)

function asd =
(
	local aux = $.rotation
	QuatToRH &aux
	return aux
)

/*fn equalVertex vt1 vt2 =
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
)*/

/*fn circumcenter p1 p2 p3 &ccenter=
(
    BC = distance p2 p3
    CA = distance p3 p1
    AB = distance p1 p2

    baryCoords = [ (BC^2 * (CA^2 + AB^2 - BC^2)), (CA^2 * (AB^2 + BC^2 - CA^2)), (AB^2 * (BC^2 + CA^2 - AB^2)) ]
    triArea = baryCoords.x + baryCoords.y + baryCoords.z
    baryCoords /= triArea

    ccenter = (baryCoords.x * p1 + baryCoords.y * p2 + baryCoords.z * p3)
)*/

/*fn getBoundingSphere Obj &BSphere =
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
							local c 
							circumcenter vtxA vtxB vtxC &c
							local r = distance c vtxA
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
	
	append BSphere center
	append BSphere radius
)*/