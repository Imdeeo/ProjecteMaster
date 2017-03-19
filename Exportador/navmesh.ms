fn TranslationToRH &translation =
(
	translation = [-translation.x, translation.z, translation.y]
)

fn exportNavMesh dir tpoly =
(
	local filename = "pathfinding.xml"
	local num_verts = getNumVerts $
	local allverts = #()

	for v = 1 to num_verts do
	(
		vert = #()
		append allverts vert
	)

	local edges = tpoly.edges

	for ed in edges do
	(
		edverts = polyop.GetVertsUsingEdge tpoly ed

		aux = #()
		for v in edverts do (
			append aux v
		
		)
		append allverts[aux[1]] aux[2] 
		append allverts[aux[2]] aux[1] 
	)

	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<path>\n" to:file
	local count = 0
	--escriure dades
	for vert in allverts do(
		count = count +1
		local auxPos = polyOp.getVert tpoly count
		TranslationToRH &auxPos
		format "\t<node name=\"node%\" pos=\"% % %\"/>\n" count auxPos[1] auxPos[2] auxPos[3] to:file
	)
	local count = 0
	--escriure dades
	for vert in allverts do(
		count = count +1
		format "\t<father name=\"node%\">\n" count to:file
			for child in vert do(
				format "\t\t<child name=\"node%\"/>\n" child to:file
			)
		format "\t</father>\n" to:file
	)
	
	
	format "</path>\n" to:file		
	close file
)


