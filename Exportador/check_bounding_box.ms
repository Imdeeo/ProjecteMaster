fileIn "Math.ms"

fn CheckBB =
(
	if classof $ != Editable_mesh then
	(
		convertToMesh $
	)
	if $BoundingBoxMax == undefined then
	(
		BoundingBoxMax = sphere()
		BoundingBoxMax.name = "BoundingBoxMax"
	)
	if $BoundingBoxMin == undefined then
	(
		BoundingBoxMin = sphere()
		BoundingBoxMin.name = "BoundingBoxMin"
	)
	if $BoundingSphere == undefined then
	(
		BoundingSphere = sphere()
		BoundingSphere.name = "BoundingSphere"
	)
	$BoundingBoxMax.radius = 0.1
	$BoundingBoxMin.radius = 0.1
	local BBMin = point3 0 0 0
	local BBMax = point3 0 0 0
	local BSphere = #()
	local InvTransform = inverse $.transform
	local nFaces = getNumFaces $	
	local BBFace = getFace $ 1
	local BBStart = (getVert $ BBFace.x)*InvTransform
	TranslationToRH &BBFace
	BBMax.x = BBStart.x
	BBMax.y = BBStart.y
	BBMax.z = BBStart.z
	BBMin.x = BBStart.x
	BBMin.y = BBStart.y
	BBMin.z = BBStart.z

	for i=1 to nFaces do
	(
		local IdxsFace = getFace $ i
		Vt1=(getVert $ IdxsFace.x)*InvTransform
		Vt2=(getVert $ IdxsFace.y)*InvTransform
		Vt3=(getVert $ IdxsFace.z)*InvTransform
		getBoundingBox Vt1 &BBMax &BBMin
		getBoundingBox Vt2 &BBMax &BBMin
		getBoundingBox Vt3 &BBMax &BBMin
	)
	
	$BoundingBoxMax.position = BBMax+$.position
	$BoundingBoxMin.position = BBMin+$.position
	
	lerp BBMin BBMax .5 &BSphere[1]
	BSphere[2] = 0

	for i=1 to nFaces do
	(
		local IdxsFace = getFace $ i
		Vt1=(getVert $ IdxsFace.x)*InvTransform
		Vt2=(getVert $ IdxsFace.y)*InvTransform
		Vt3=(getVert $ IdxsFace.z)*InvTransform
		getBoundingSphere Vt1 &BSphere
		getBoundingSphere Vt2 &BSphere
		getBoundingSphere Vt3 &BSphere
	)
	
	$BoundingSphere.position = BSphere[1]+$.position
	$BoundingSphere.radius = BSphere[2]
)

CheckBB()