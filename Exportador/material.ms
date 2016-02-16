fileIn "math.ms"
fileIn "utils.ms"

fn getMaterials arx Obj =
(
	local n_materials=1
	if (Obj.material!=undefined)then
	(
		if(classof(Obj.material)==Multimaterial)then
		(
			n_materials=Obj.material.numsubs
			writeLong arx n_materials
			for i=1 to Obj.material.numsubs do
			(
				WriteShort arx Obj.material[i].name.count #unsigned
				writeString arx Obj.material[i].name
			)
		)
		else
		(
			writeLong arx 1
			WriteShort arx Obj.material.name.count #unsigned
			writeString arx Obj.material.name
		)
	)
	else
	(
		n_materials = 0
	)
)

fn createMaterials dir filename Objs copyTexture =
(	
	local pathfile
	local relativePathfile
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	if Objs != undefined then
	(
		materialsDictionary = dotNetObject "System.Collections.Hashtable"
		for i=1 to Objs.count do
		(
			mat = #()
			n_materials = 1
			
			if (Objs[i].material!=undefined) then
			(
				if(classof(Objs[i].material)==Multimaterial)then
				(
					mat = Objs[i].material
					n_materials = Objs[i].material.numsubs
				)
				else
				(
					append mat Objs[i].material
				)
				
				for j=1 to n_materials do
				(
					if materialsDictionary.Item[mat[j].name]==undefined then
					(
						format "\t<material name=\"%\" renderable_object_technique=\"diffuse_technique\">\n" mat[j].name to:file
						materialsDictionary.Add mat[j].name "defined"
						if mat[j].diffuseMap != undefined then  
						(
							if (classof mat[j].diffuseMap == Bitmaptexture) and (mat[j].diffuseMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].diffuseMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile mat[j].diffuseMap.bitmap.filename pathfile
								)
							)
						)
						format "\t</material>\n" to:file 
					)
				)
			)
		)
	)
	format "</materials>\n" to:file		
	close file
)