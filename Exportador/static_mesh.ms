fileIn "Math.ms"
fileIn "Utils.ms"

fn createStaticMesh dir level filename Objs=
(	
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	if Objs != undefined then
	(
		for i=1 to Objs.count do
		(
			if Objs[i].material != undefined then format "\t<static_mesh name=\"%\" filename=\"%\" physx_mesh_directory=\"%\"/>\n" Objs[i].name ("Data\\" + level + "\\Meshes\\"+Objs[i].name+".mesh") ("Data\\" + level + "\\Meshes\\PhysXMeshes") to:file
		)
	)
	format "</static_meshes>\n" to:file
	close file
)
