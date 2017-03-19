tpoly = $
num_verts = getNumVerts $
allverts = #()

for v = 1 to num_verts do
(
    vert = getVert tmesh v
    append allverts vert
)
print allverts

format "\n" --to:out_file
edges = tmesh.edges

for ed in edges do
(
	print ed
	edverts = polyop.GetVertsUsingEdge tpoly ed
	print "ed verts: "
	for v in edverts do (
		print allverts[v]
	)
	print "\n"
)