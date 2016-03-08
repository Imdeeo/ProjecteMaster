fileIn "math.ms"
fileIn "utils.ms"

fn getMaterials arx Obj =
(
	local n_materials = 1
	if (Obj.material != undefined)then
	(
		if(classOf(Obj.material)==Multimaterial)then
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
					if (materialsDictionary.Item[mat[j].name]==undefined) then
					(
						if (classof mat[j].diffuseMap == Bitmaptexture) then
						(
							if (classof mat[j].reflectionMap == Bitmaptexture) then
							(
								format "\t<material name=\"%\" renderable_object_technique=\"reflection\">\n" mat[j].name to:file
							)else(
								format "\t<material name=\"%\" renderable_object_technique=\"lights\">\n" mat[j].name to:file
							)
						)
						materialsDictionary.Add mat[j].name "defined"
						if (classof mat[j].diffuseMap == Bitmaptexture) then
						(
							if (mat[j].diffuseMap.bitmap != undefined) then
							(
								--nomTextura = (openBitMap mat[j].diffuseMap.bitmap.filename).fileName
								nomTextura = filenameFromPath (mat[j].diffuseMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"diffuse\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile (openBitMap mat[j].diffuseMap.bitmap.filename).fileName pathfile
									--copyFile mat[j].diffuseMap.bitmap.filename pathfile
								)
							)
						)
						if (classof mat[j].selfIllumMap == Bitmaptexture) then
						(
							if (mat[j].selfIllumMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].selfIllumMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"lightmap\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile mat[j].selfIllumMap.bitmap.filename pathfile
								)
							)
						)
						if (classof mat[j].reflectionMap == Bitmaptexture) then
						(
							if (mat[j].reflectionMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].reflectionMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"reflection\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile mat[j].reflectionMap.bitmap.filename pathfile
								)
							)
						)
						if (classof mat[j].bumpMap == Bitmaptexture) then
						(
							if (mat[j].bumpMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].bumpMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"normal\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile mat[j].bumpMap.bitmap.filename pathfile
								)
							)
						) else if (classof mat[j].bumpMap == Normal_Bump) then
						(
							if (mat[j].bumpMap.normal_map.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].bumpMap.normal_map.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"normal\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile mat[j].bumpMap.normal_map.bitmap.filename pathfile
								)
							)
						)
						if (classof mat[j].diffuseMap == Bitmaptexture) then
						(
							local specularAmount = mat[j].specularLevelMapAmount/100
							local glossinessAmount = mat[j].glossiness
							format "\t\t<parameter type=\"float\" name=\"active\" value=\"1.0\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"exposure\" value=\"0.082\" description=\"min=0.0 max=3.0 step=0.001\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"specular_power\" value=\"%\" description=\"min=0.0 max=100.0 step=0.1\"/>\n" glossinessAmount to:file
							format "\t\t<parameter type=\"float\" name=\"specular_factor\" value=\"%\" description=\"min=0.0 max=1.0 step=0.1\"/>\n" specularAmount to:file
							
							format "\t</material>\n" to:file 
						)
					)
				)
			)
		)
	)
	format "</materials>\n" to:file		
	close file
)