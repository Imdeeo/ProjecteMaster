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
			writeLong arx n_materials
			WriteShort arx Obj.material.name.count #unsigned
			writeString arx Obj.material.name
		)
	)
	else
	(
		writeLong arx n_materials
		WriteShort arx 11 #unsigned
		writeString arx "no_material"
	)
)

fn createMaterials dir filename Objs copyTexture =
(	
	local pathfile
	local relativePathfile
	file=createfile (dir+filename)
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	format "\t<material name=\"no_material\" renderable_object_technique=\"lights\"/>\n" to:file
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
						local hasDiffuse = (classof mat[j].diffuseMap == Bitmaptexture) as booleanClass
						local hasReflection = (classof mat[j].reflectionMap == Bitmaptexture) as booleanClass
						local hasBump = (classof mat[j].bumpMap == Bitmaptexture or classof mat[j].bumpMap ==  Normal_Bump) as booleanClass
						local hasLightmap = (classof mat[j].selfIllumMap == Bitmaptexture) as booleanClass
						local hasRNM = (classof mat[j].selfIllumMap == Multi_Sub_Map) as booleanClass
						local hasGlossiness = (classof mat[j].glossinessMap == Bitmaptexture) as booleanClass
						local hasSpecular = (classof mat[j].specularMap == Bitmaptexture) as booleanClass
						
						local renderableTechnique = ""
						if (hasDiffuse and not hasReflection and not hasBump and not hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lights"
						)
						else if (not hasDiffuse and hasReflection and not hasBump and hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "skybox"
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "reflection"
						)
						else if (hasDiffuse and not hasReflection and hasBump and not hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "bump"
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and not hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "glossiness"
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and not hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "specularmap"
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "bump_env"
						)
						else if (hasDiffuse and not hasReflection and hasBump and hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lightmap_bump"
						)						
						else if (hasDiffuse and hasReflection  and hasBump and hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lightmap_bump_env"
						)
						else if (hasDiffuse and hasReflection  and not hasBump and hasLightmap and not hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lightmap_env"
						)						
						else if (hasDiffuse and hasReflection and hasBump and hasLightmap and not hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "lightmap_bump_specularmap"
						)
						else if (hasDiffuse and hasReflection and not hasBump and hasLightmap and not hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lightmap_glossiness"
						)
						else if (hasDiffuse and hasReflection and not hasBump and hasLightmap and not hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "lightmap_specularmap"
						)
						else if (hasDiffuse and hasReflection and hasBump and hasLightmap and not hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "lightmap_bump_glossiness"
						)
						else if (hasDiffuse and not hasReflection and not hasBump and not hasLightmap and hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity"
						)
						else if (hasDiffuse and not hasReflection and hasBump and not hasLightmap and hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity_bump"
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity_bump_env"
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and hasRNM and not hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity_env"
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity_bump_glossiness"							
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "radiosity_bump_specularmap"
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "radiosity_glossiness"							
						)
						else if (hasDiffuse and hasReflection and not hasBump and not hasLightmap and hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "radiosity_specularmap"
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and not hasRNM and hasGlossiness and hasSpecular) then
						(
							renderableTechnique = "bump_specularmap"
						)
						else if (hasDiffuse and hasReflection and hasBump and not hasLightmap and not hasRNM and hasGlossiness and not hasSpecular) then
						(
							renderableTechnique = "bump_glossiness"
						)
						else
						(
							renderableTechnique = "lights"
						)

						format "\t<material name=\"%\" renderable_object_technique=\"%\">\n" mat[j].name renderableTechnique to:file
						
						materialsDictionary.Add mat[j].name "defined"
						if (hasDiffuse == true) then
						(
							if (mat[j].diffuseMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].diffuseMap.bitmap.filename)					
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"diffuse\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile (openBitMap mat[j].diffuseMap.bitmap.filename).fileName pathfile
								)
							)
						)
						
						if (hasRNM) then
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
								copyFile (openBitMap mat[j].selfIllumMap.id_0_color_shader.bitmap.filename).fileName pathfile1
								copyFile (openBitMap mat[j].selfIllumMap.id_1_color_shader.bitmap.filename).fileName pathfile2
								copyFile (openBitMap mat[j].selfIllumMap.id_2_color_shader.bitmap.filename).fileName pathfile3
							)
						)
						else if (hasLightmap) then
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
									copyFile (openBitMap mat[j].selfIllumMap.bitmap.filename).fileName pathfile
								)
							)
						)
						
						if (hasReflection) then
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
									copyFile (openBitMap mat[j].reflectionMap.bitmap.filename).fileName pathfile
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
									copyFile (openBitMap mat[j].bumpMap.bitmap.filename).fileName pathfile
								)
							)
						) 
						else if (classof mat[j].bumpMap == Normal_Bump) then
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
									copyFile (openBitMap mat[j].bumpMap.normal_map.bitmap.filename).fileName pathfile
								)
							)
						)
						
						if (hasGlossiness and not hasSpecular) then
						(
							if (mat[j].glossinessMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].glossinessMap.bitmap.filename)	
								pathfile = (dir + "Textures\\" + nomTextura)
								relativePathfile = pathfile
								getRelativePath &relativePathFile
								format "\t\t<texture type=\"glossiness\" filename=\"%\"/>\n" relativePathfile to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile (openBitMap mat[j].glossinessMap.bitmap.filename).fileName pathfile
								)
							)
						)
						else if (hasGlossiness and hasSpecular) then
						(
							if (mat[j].specularMap.bitmap != undefined and mat[j].glossinessMap.bitmap != undefined) then
							(
								nomTextura = filenameFromPath (mat[j].glossinessMap.bitmap.filename)	
								nomTextura2 = filenameFromPath (mat[j].specularMap.bitmap.filename)	
								pathfile = (dir + "Textures\\" + nomTextura)
								pathfile2 = (dir + "Textures\\" + nomTextura2)
								relativePathfile = pathfile
								relativePathfile2 = pathfile2
								getRelativePath &relativePathFile
								getRelativePath &relativePathFile2
								format "\t\t<texture type=\"glossiness\" filename=\"%\"/>\n" relativePathfile to:file
								format "\t\t<texture type=\"specular\" filename=\"%\"/>\n" relativePathfile2 to:file
								if copyTexture == true then
								(
									dirExist (dir+"Textures")
									copyFile (openBitMap mat[j].glossinessMap.bitmap.filename).fileName pathfile
									copyFile (openBitMap mat[j].specularMap.bitmap.filename).fileName pathfile2
								)
							)
						)
						
						if (hasDiffuse) then
						(
							local specularAmount = mat[j].specularLevelMapAmount/100
							local glossinessAmount = mat[j].glossiness
						
							format "\t\t<parameter type=\"float\" name=\"active\" value=\"1.0\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"exposure\" value=\"0.80\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" to:file
							if (hasGlossiness and not hasSpecular) then
							(
								format "\t\t<parameter type=\"float\" name=\"glossiness\" value=\"%\" description=\"min=0.0 max=100.0 step=0.1\"/>\n" glossinessAmount to:file
								format "\t\t<parameter type=\"float\" name=\"reflectivity\" value=\"%\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" specularAmount to:file
							)							
							else
							(
								format "\t\t<parameter type=\"float\" name=\"specular_power\" value=\"%\" description=\"min=0.0 max=100.0 step=0.1\"/>\n" glossinessAmount to:file
								format "\t\t<parameter type=\"float\" name=\"specular_factor\" value=\"0.2\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" specularAmount to:file
							)							
							format "\t\t<parameter type=\"float\" name=\"reflection\" value=\"1.0\" description=\"min=0.0 max=1.0 step=0.001\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"ssr_reflection\" value=\"0.0\" description=\"min=0.0 max=1.0 step=0.001\"/>\n" to:file
							format "\t\t<parameter type=\"float\" name=\"bump_factor\" value=\"1.0\" description=\"min=0.0 max=2.0 step=0.05\"/>\n" to:file
							if (not hasSpecular) then
							(
								format "\t\t<parameter type=\"float\" name=\"metalness\" value=\"0.0\" description=\"min=0.0 max=1.0 step=0.01\"/>\n" to:file
								format "\t\t<parameter type=\"color\" name=\"metal_color\" value=\"1.0 1.0 1.0 0.0\" description=\"\"/>\n" to:file
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