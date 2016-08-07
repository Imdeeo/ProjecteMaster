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
								if (classof mat[j].bumpMap == Bitmaptexture or classof mat[j].bumpMap ==  Normal_Bump) then
								(
									if (classof mat[j].selfIllumMap == Bitmaptexture) then
									(
										format "\t<material name=\"%\" renderable_object_technique=\"lightmap_bump\">\n" mat[j].name to:file
									)
									else if (classof mat[j].selfIllumMap == Multi_Sub_Map) then
									(
										format "\t<material name=\"%\" renderable_object_technique=\"radiosity_bump\">\n" mat[j].name to:file
									)
									else
									(
										format "\t<material name=\"%\" renderable_object_technique=\"bump\">\n" mat[j].name to:file
									)
								)
								else
								(
									if (classof mat[j].selfIllumMap == Bitmaptexture) then
									(
										format "\t<material name=\"%\" renderable_object_technique=\"lightmap\">\n" mat[j].name to:file
									)
									else if (classof mat[j].selfIllumMap == Multi_Sub_Map) then
									(
										format "\t<material name=\"%\" renderable_object_technique=\"radiosity\">\n" mat[j].name to:file
									)
									else
									(
										format "\t<material name=\"%\" renderable_object_technique=\"lights\">\n" mat[j].name to:file
									)
								)
							)
						) else (
							if (classof mat[j].reflectionMap == Bitmaptexture) then
							(
								format "\t<material name=\"%\" renderable_object_technique=\"skybox\">\n" mat[j].name to:file
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
						if (classof mat[j].selfIllumMap == Multi_Sub_Map) then
						(
							nomTextura1 = filenameFromPath (mat[j].selfIllumMap.id_0_color_shader.filename)
							nomTextura2 = filenameFromPath (mat[j].selfIllumMap.id_1_color_shader.filename)
							nomTextura3 = filenameFromPath (mat[j].selfIllumMap.id_2_color_shader.filename)
							pathfile1 = (dir + "Textures\\" + nomTextura1)
							pathfile2 = (dir + "Textures\\" + nomTextura2)
							pathfile3 = (dir + "Textures\\" + nomTextura3)
							relativePathfile1 = pathfile1
							relativePathfile2 = pathfile2
							relativePathfile3 = pathfile3
							getRelativePath &relativePathFile1
							getRelativePath &relativePathFile2
							getRelativePath &relativePathFile3
							format "\t\t<texture type=\"lightmap\" filename=\"%\"/>\n" relativePathfile1 to:file
							format "\t\t<texture type=\"lightmap2\" filename=\"%\"/>\n" relativePathfile2 to:file
							format "\t\t<texture type=\"lightmap3\" filename=\"%\"/>\n" relativePathfile3 to:file
							if copyTexture == true then
							(
								dirExist (dir+"Textures")
								copyFile mat[j].selfIllumMap.id_0_color_shader.filename pathfile1
								copyFile mat[j].selfIllumMap.id_1_color_shader.filename pathfile2
								copyFile mat[j].selfIllumMap.id_2_color_shader.filename pathfile3
							)
						)
						else if (classof mat[j].selfIllumMap == Bitmaptexture) then
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
							format "\t\t<parameter type=\"float\" name=\"exposure\" value=\"0.80\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"specular_power\" value=\"%\" description=\"min=0.0 max=100.0 step=0.1\"/>\n" glossinessAmount to:file
							format "\t\t<parameter type=\"float\" name=\"specular_factor\" value=\"%\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" specularAmount to:file
							format "\t\t<parameter type=\"float\" name=\"reflection\" value=\"1.0\" description=\"min=0.0 max=1.0 step=0.001\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"ssr_reflection\" value=\"0.0\" description=\"min=0.0 max=1.0 step=0.001\"/>\n" to:file
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