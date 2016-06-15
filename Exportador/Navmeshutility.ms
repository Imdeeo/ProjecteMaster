fileIn "navmesh.ms"

utility NavMeshUtility "NavMeshUtility" width:168 height:150(
	button SetPathButton "Set Path" pos:[8,8] width:152 height:40
	label PathLabel "Label" pos:[8,56] width:152 height:64s
	button ExportNavButton "Export" pos:[16,100] width:136 height:32
	
	fn get_Dir =
	(
		local dir = ""+PathLabel.text+"\\" 
		dirExist dir	
		dir
	)
		
	on NavMeshUtility open do
	(
		PathLabel.text="F:\\Game\\Data"
	)
	
	on SetPathButton pressed do
	(
		local l_Path=getSavePath caption:"Select game folder" initialDir:PathLabel.text
		if l_Path!=undefined then
		(
			PathLabel.text=l_Path
		)
	)
	
	on ExportNavButton pressed do
    (
		if $ == undefined then
		(
			messageBox "You must select your NavigationMesh" title:"ERROR"
		)
		else
		(
			exportNavMesh (get_Dir()) $
		)
    )
	
)
