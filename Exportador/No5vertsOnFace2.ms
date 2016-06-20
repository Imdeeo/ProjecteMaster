utility ReviseMeshUtility "ReviseMeshUtility" width:168 height:150
(
	button ReviseButton "Revise" pos:[15,89] width:136 height:32
			
	label artistasLabel "Label" pos:[17,13] width:129 height:56
	
	on ReviseMeshUtility open do
	(
		artistasLabel.text="Hi artista! Animo que esta vez seguro que no hay caras de 5 vértices!"
	)
	
	on ReviseButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona un objeto melón!" title:"ERROR"
		)
		else
		(
			Revisar $
		)
	)
	
	fn Revisar tPoly = 
	(
		faces = #()
		local count= 0
		for i = 1 to (polyOp.getNumFaces) tPoly do
		(
			numVerts = polyOp.getFaceDeg tPoly i
			if (numVerts > 4) then
			(
				count = count + 1
				append faces i
			)		
		)
		if (count > 0) then
		(
			messageBox "Ale, a revisar" title:"Meeeeeeeeec"
			polyOp.setFaceSelection tPoly faces
		)
		else
		(
			messageBox "OH YEAAAAAAAAAAAAAAH" title:"Felicidades"
		)
	)

)

