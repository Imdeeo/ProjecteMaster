Obj = undefined
callbacks.removeScripts #selectionSetChanged id:#swapMode
callbacks.addScript #selectionSetChanged "UABSetProperties.swapMode $" id:#swapMode persistent:false
	
rollout UABSetProperties "UABSetProperties" width:280 height:394
(
	GroupBox grp1 "Geometry" pos:[8,8] width:265 height:328
		label GeometryLabel "Nothing selected" pos:[24,32] width:232 height:24
		GroupBox RenderGroup "Render" pos:[16,56] width:248 height:120
			checkbox RenderExportBox "Export" pos:[24,72] width:88 height:16
			dropdownList RenderExportList "" pos:[120,72] width:136 height:21 enabled:false items:#("instance_mesh", "animated_instance_mesh")
			checkbox RenderVisibleBox "Visible" pos:[24,96] width:88 height:16 enabled:false
			checkbox RenderLayerBox "Layer" pos:[24,120] width:88 height:16 enabled:false
			checkbox CinematicBox "Cinematic" pos:[24,144] width:88 height:16 enabled:false 
			dropdownList RenderLayerList "" pos:[120,112] width:136 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "skybox")
			edittext AnimModelText "AnimModelLabel" pos:[120,144] width:136 height:24 enabled:false
		GroupBox PhysicsGroup "Physics" pos:[16,176] width:248 height:104
			checkbox PhysicsGenerateBox "Generate" pos:[24,200] width:88 height:16
			dropdownList PhysicsGenerateList "" pos:[120,200] width:136 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
			checkbox PhysicsLayerBox "Layer" pos:[24,224] width:88 height:16 enabled:false
			dropdownList PhysicsLayerList "" pos:[120,224] width:136 height:21 enabled:false items:#("logic")
			edittext PhysicsMaterialText "Material" pos:[24,248] width:230 height:24 enabled:false
		GroupBox LogicGroup "Logic" pos:[16,280] width:248 height:48
			checkbox LogicGenerateBox "Generate" pos:[24,304] width:88 height:16
			dropdownList LogicGenerateList "" pos:[120,304] width:136 height:21 items:#("stairs")
		button GetButton "Get" pos:[24,348] width:112 height:32
		button SetButton "Set" pos:[144,348] width:112 height:32
	GroupBox grp2 "Lights" pos:[8,8] width:265 height:208 visible:false
		GroupBox LightGroup "Light" pos:[16,24] width:248 height:100
			label LightLabel "Nothing selected" pos:[24,42] width:232 height:24 visible:false
			checkbox LightExportBox "Export" pos:[24,68] width:88 height:16 visible:false
			checkbox LightEnabledBox "Enabled" pos:[24,88] width:88 height:16 visible:false enabled:false
		GroupBox ShadowmapGroup "Shadowmap" pos:[16,126] width:248 height:80
			checkbox ShadowmapExportBox "Export" pos:[24,144] width:88 height:16 visible:false
			dropdownList ShadowmapLayerList "" pos:[120,144] width:136 height:21 visible:false enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
			label ShadowmapWHLabel "Width and height" pos:[24,171] width:88 height:16
			edittext ShadowmapWText pos:[117,169] width:40 height:20 visible:false enabled:false text: "1024"
			edittext ShadowmapHText pos:[157,169] width:40 height:20 visible:false enabled:false text: "1024"
		button GetLightButton "Get" pos:[24,224] width:112 height:32 visible:false
		button SetLightButton "Set" pos:[144,224] width:112 height:32 visible:false
	GroupBox grp3 "Inactive" pos:[8,8] width:265 height:61 visible:false
		label UnsupportedLabel "Current selection is not supported." pos:[24,32] width:232 height:24 visible:false
	GroupBox grp4 "Cameras" pos:[8,8] width:265 height:88 visible:false
		label CameraLabel "Nothing selected" pos:[24,32] width:232 height:24 visible:false
		checkbox CameraExportBox "Export" pos:[24,64] width:88 height:16 visible:false
		dropdownList CameraExportList "" pos:[120,62] width:136 height:21 enabled:false items:#("fps", "spherical", "cycle", "reverse")
		button GetCameraButton "Get" pos:[24,104] width:112 height:32 visible:false
		button SetCameraButton "Set" pos:[144,104] width:112 height:32 visible:false
	
	fn toggleGeometryGroup bool =
	(
		grp1.visible = bool
		GeometryLabel.visible = bool
		RenderGroup.visible = bool
		RenderExportBox.visible = bool
		RenderExportList.visible = bool
		RenderVisibleBox.visible = bool
		RenderLayerBox.visible = bool
		RenderLayerList.visible = bool
		CinematicBox.visible = bool
		AnimModelText.visible = bool
		PhysicsGroup.visible = bool
		PhysicsGenerateBox.visible = bool
		PhysicsGenerateList.visible = bool
		PhysicsLayerBox.visible = bool
		PhysicsLayerList.visible = bool
		PhysicsMaterialText.visible = bool
		LogicGroup.visible = bool
		LogicGenerateBox.visible = bool
		LogicGenerateList.visible = bool
		GetButton.visible = bool
		SetButton.visible = bool
	)
	
	fn toggleLightsGroup bool =
	(
		grp2.visible = bool
		LightGroup.visible = bool
		LightLabel.visible = bool
		LightExportBox.visible = bool
		LightEnabledBox.visible = bool
		ShadowmapGroup.visible = bool
		ShadowmapExportBox.visible = bool
		ShadowmapLayerList.visible = bool
		ShadowmapWHLabel.visible = bool
		ShadowmapWText.visible = bool
		ShadowmapHText.visible = bool
		GetLightButton.visible = bool
		SetLightButton.visible = bool
	)
	
	fn toggleCamerasGroup bool =
	(
		grp4.visible = bool
		CameraLabel.visible = bool
		CameraExportBox.visible = bool
		CameraExportList.visible = bool
		GetCameraButton.visible = bool
		SetCameraButton.visible = bool
	)
	
	fn toggleUnsupportedGroup bool =
	(
		grp3.visible = bool
		UnsupportedLabel.visible = bool
	)
	
	fn toggleRender bool =
	(
		RenderExportList.enabled = bool
		RenderVisibleBox.enabled = bool
		RenderLayerBox.enabled = bool
		RenderLayerList.enabled = bool
		AnimModelText.enabled = bool
		CinematicBox.enabled = bool
	)
	
	fn togglePhysics bool =
	(
		PhysicsGenerateList.enabled = bool
		PhysicsLayerBox.enabled =  bool
		PhysicsLayerList.enabled =  bool
		PhysicsMaterialText.enabled = bool
	)
	
	fn toggleLights bool =
	(
		LightEnabledBox.enabled = bool
	)
	
	fn toggleShadowmap bool =
	(
		ShadowmapLayerList.enabled = bool
		ShadowmapWHLabel.enabled = bool
		ShadowmapWText.enabled = bool
		ShadowmapHText.enabled = bool
	)
	
	fn toggleCameras bool =
	(
		CameraExportList.enabled = bool
	)
	
	fn loadUnsupportedSelection Obj =
	(
		toggleLightsGroup false
		toggleGeometryGroup false
		toggleUnsupportedGroup true
		toggleCamerasGroup false
		UABSetProperties.height = 80
		if (Obj == undefined) then
		(
			UnsupportedLabel.text = "Nothing selected."
		) else (
			UnsupportedLabel.text = ""+(ClassOf Obj) as string+" is not a supported selection."
		)
	)
	
	fn loadLightMode Obj =
	(
		if ClassOf Obj == ObjectSet then
		(
			local Objs = Obj
			local textLabel = "Light Set"
		) else (
			local Objs = #()
			append Objs Obj
			local textLabel = Obj.name+" ("+(ClassOf Obj) as string+")"
		)
		
		local allValid = true
		
		for i = 1 to Objs.count do
		(
			if (ClassOf Objs[i] != Omnilight) and
			(ClassOf Objs[i] != targetSpot) and
			(ClassOf Objs[i] != freeSpot) and
			(ClassOf Objs[i] != Directionallight) and
			(ClassOf Objs[i] != TargetDirectionallight)
			then (
				allValid = false
			)
		)
		
		if allValid then (
			toggleLightsGroup true
			toggleGeometryGroup false
			toggleUnsupportedGroup false
			toggleCamerasGroup false
			LightLabel.text = textLabel
			UABSetProperties.height = 268
		) else (
			loadUnsupportedSelection Objs
		)
	)
	
	fn loadGeometryMode Obj =
	(
		if ClassOf Obj == ObjectSet then
		(
			local Objs = Obj
			local textLabel = "Geometry Set"
		) else (
			local Objs = #()
			append Objs Obj
			local textLabel = Obj.name+" ("+(ClassOf Obj) as string+")"
		)
		
		local allValid = true
		for i = 1 to Objs.count do
		(
			if (findString Objs[i].name ".Target" != undefined) then
			(
				allValid = false
			)
		)
	
		if allValid then
		(
			toggleLightsGroup false
			toggleGeometryGroup true
			toggleUnsupportedGroup false
			toggleCamerasGroup false
			GeometryLabel.text = textLabel
			UABSetProperties.height = 394
		)
		else
		(
			loadUnsupportedSelection Objs
		)
	)
	
	fn loadCameraMode Obj =
	(
		if ClassOf Obj == ObjectSet then
		(
			local Objs = Obj
			local textLabel = "Camera Set"
		) else (
			local Objs = #()
			append Objs Obj
			local textLabel = Obj.name+" ("+(ClassOf Obj) as string+")"
		)
		toggleLightsGroup false
		toggleGeometryGroup false
		toggleUnsupportedGroup false
		toggleCamerasGroup true
		CameraLabel.text = textLabel
		UABSetProperties.height = 150
	)
	
	fn loadSetMode Objs =
	(
		local allCamera = true
		local allLight = true
		local allGeometry = true
		for i = 1 to Objs.count do
		(
			allCamera = allCamera and (SuperClassOf Objs[i] == camera)
			allLight = allLight and (SuperClassOf Objs[i] == light)
			allGeometry = allGeometry and (SuperClassOf Objs[i] == GeometryClass)
		)
		if allCamera then loadCameraMode Objs
		else if allLight then loadLightMode Objs
		else if allGeometry then loadGeometryMode Objs
		else loadUnsupportedSelection Objs
	)
		
	fn swapMode Obj =
	(
		case SuperClassOf Obj of
		(
			camera: loadCameraMode Obj
			light: loadLightMode Obj
			GeometryClass:  loadGeometryMode Obj
			Set: loadSetMode Obj
			default: loadUnsupportedSelection Obj
		)
	)
	
	fn initUserProps Obj =
	(
		if(SuperClassOf Obj == GeometryClass) then
		(
			setUserProp Obj "render_export" true
			setUserProp Obj "render_export_type" "instance_mesh"
			setUserProp Obj "render_visible" true
			setUserProp Obj "has_render_layer" true
			setUserProp Obj "render_layer" "solid"
			setUserProp Obj "anim_model_core" "undefined"
			setUserProp Obj "physics_generate" false
			setUserProp Obj "physics_generate_type" "triangle_mesh"
			setUserProp Obj "has_physics_layer" false
			setUserProp Obj "physics_layer" (1 as string)
			setUserProp Obj "physics_material" ""
			setUserProp Obj "logic_generate" false
			setUserProp Obj "logic_generate_type" "stairs"
			setUserProp OBj "cinematic_object" false
		) else if(SuperClassOf Obj == light) then
		(
			setUserProp Obj "light_export" true
			setUserProp Obj "light_enabled" true
			setUserProp Obj "shadowmap_export" false
			setUserProp Obj "shadowmap_layer" "solid"
			setUserProp Obj "shadowmap_w" (1024 as string)
			setUserProp Obj "shadowmap_h" (1024 as string)
		) else if(SuperClassOf Obj == camera) then
		(
			setUserProp Obj "camera_export" true
			setUserProp Obj "camera_export_type" "static"
		)
	)
	
	fn GetProperties Obj =
	(
		if ClassOf Obj == ObjectSet then
		(
			local Objs = Obj
		) else (
			local Objs = #()
			append Objs Obj
		)
		for i = 1 to Objs.count do
		(
			if (getUserProp Objs[i] "render_export" == undefined) and (getUserProp Objs[i] "light_export" == undefined) and (getUserProp Objs[i] "camera_export" == undefined) then
			(
				initUserProps Objs[i]
			)
		)	
		if(SuperClassOf Objs[1] == GeometryClass) then
		(
			RenderExportBox.checked = getUserProp Objs[1] "render_export"
			if RenderExportBox.checked == true then
			(
				toggleRender true
				
				case getUserProp Objs[1] "render_export_type" of
					(
						"instance_mesh": RenderExportList.selection = 1
						"animated_instance_mesh": RenderExportList.selection = 2
					)
				
				RenderVisibleBox.checked = getUserProp Objs[1] "render_visible"
				
				RenderLayerBox.checked = getUserProp Objs[1] "has_render_layer"
				case getUserProp Objs[1] "render_layer" of
					(
						"solid": RenderLayerList.selection = 1
						"alpha_objects": RenderLayerList.selection = 2
						"alpha_blend_objects": RenderLayerList.selection = 3
						"particles": RenderLayerList.selection = 4
					)
			
				AnimModelText.text = getUserProp Objs[1] "anim_model_core"
				CinematicBox.checked = getUserProp Objs[1] "cinematic_object"
			) else (
				toggleRender false
				RenderVisibleBox.checked = false
				RenderLayerBox.checked = false
				CinematicBox.checked = false
			)
			
			PhysicsGenerateBox.checked = getUserProp Objs[1] "physics_generate"
			if PhysicsGenerateBox.checked == true then
			(
				togglePhysics true
				case getUserProp Objs[1] "physics_generate_type" of
					(
						"triangle_mesh": PhysicsGenerateList.selection = 1
						"convex_shape": PhysicsGenerateList.selection = 2
						"box_shape": PhysicsGenerateList.selection = 3
						"sphere_shape": PhysicsGenerateList.selection = 4
						"plane_shape": PhysicsGenerateList.selection = 5
						"capsule_shape": PhysicsGenerateList.selection = 6
					)
			
				PhysicsLayerBox.checked = getUserProp Objs[1] "has_physics_layer"
				case getUserProp Objs[1] "physics_layer" of
					(
						"logic": PhysicsLayerList.selection = 1
					)
					
				PhysicsMaterial.text = getUserProp Objs[1] "physics_material"
			) else (
				
				togglePhysics false
				PhysicsLayerBox.checked = false
				PhysicsMaterialText.text = ""
			)
			
			LogicGenerateBox.checked = getUserProp Objs[1] "logic_generate"
			case getUserProp Objs[1] "logic_generate_type" of
				(
					"stairs": LogicGenerateList.selection = 1
				)
		) else if(SuperClassOf Objs[1] == light) then
		(
			LightExportBox.checked = getUserProp Objs[1] "light_export"
			toggleLights (LightExportBox.checked)
			LightEnabledBox.checked = getUserProp Objs[1] "light_enabled"
			ShadowmapExportBox.checked = getUserProp Objs[1] "shadowmap_export"
			toggleShadowmap (ShadowmapExportBox.checked)
			case getUserProp Objs[1] "shadowmap_layer" of
				(
					"solid": ShadowmapLayerList.selection = 1
					"alpha_objects": ShadowmapLayerList.selection = 2
					"alpha_blend_objects": ShadowmapLayerList.selection = 3
					"particles": ShadowmapLayerList.selection = 4
				)
			ShadowmapWText.text = (getUserProp Objs[1] "shadowmap_w") as string
			ShadowmapHText.text = (getUserProp Objs[1] "shadowmap_h") as string
			
		) else if(SuperClassOf Objs[1] == camera) then
		(
			CameraExportBox.checked = getUserProp Objs[1] "camera_export"
			toggleCameras (CameraExportBox.checked)
			case getUserProp Objs[1] "camera_export_type" of
				(
					"fps": CameraExportList.selection = 1
					"spherical": CameraExportList.selection = 2
					"cycle": CameraExportList.selection = 3
					"reverse": CameraExportList.selection = 4
				)
		)
	)
	
	fn SetProperties Obj =
	(
		if ClassOf Obj == ObjectSet then
		(
			local Objs = Obj
		) else (
			local Objs = #()
			append Objs Obj
		)
		
		for i = 1 to Objs.count do
		(
			if(SuperClassOf Objs[i] == GeometryClass) then
			(
				if RenderExportBox.checked then
				(
					setUserProp Objs[i] "render_export" true
					
					case RenderExportList.items[RenderExportList.selection] of
					(
						"instance_mesh": setUserProp Objs[i] "render_export_type" "instance_mesh"
						"animated_instance_mesh": setUserProp Objs[i] "render_export_type" "animated_instance_mesh"
					)	
				
				) else (
					setUserProp Objs[i] "render_export" false
				)
				
				if RenderVisibleBox.checked then
				(
					setUserProp Objs[i] "render_visible" true
				) else (
					setUserProp Objs[i] "render_visible" false
				)
				
				if RenderLayerBox.checked then
				(
					setUserProp Objs[i] "has_render_layer" true
					
					case RenderLayerList.items[RenderLayerList.selection] of
					(
						"solid": setUserProp Objs[i] "render_layer" "solid"
						"alpha_objects": setUserProp Objs[i] "render_layer" "alpha_objects"
						"alpha_blend_objects": setUserProp Objs[i] "render_layer" "alpha_blend_objects"
						"particles": setUserProp Objs[i] "render_layer" "particles"
					)	
					
				) else (
					setUserProp Objs[i] "has_render_layer" false
				)
				
				if AnimModelText.text == "" then
				(
					setUserProp Objs[i] "anim_model_core" "undefined"
				) else (
					setUserProp Objs[i] "anim_model_core" AnimModelText.text
				)
				
				if PhysicsGenerateBox.checked then
				(
					setUserProp Objs[i] "physics_generate" true
					
					case PhysicsGenerateList.items[PhysicsGenerateList.selection] of
					(
						"triangle_mesh": setUserProp Objs[i] "physics_generate_type" "triangle_mesh"
						"convex_shape": setUserProp Objs[i] "physics_generate_type" "convex_shape"
						"box_shape": setUserProp Objs[i] "physics_generate_type" "box_shape"
						"sphere_shape": setUserProp Objs[i] "physics_generate_type" "sphere_shape"
						"plane_shape": setUserProp Objs[i] "physics_generate_type" "plane_shape"
						"capsule_shape": setUserProp Objs[i] "physics_generate_type" "capsule_shape"
					)	
				
				) else (
					setUserProp Objs[i] "physics_generate" false
				)
				
				if PhysicsLayerBox.checked then
				(
					setUserProp Objs[i] "has_physics_layer" true
					
					case PhysicsLayerList.items[PhysicsLayerList.selection] of
					(
						"logic": setUserProp Objs[i] "physics_layer" (1 as string)
					)	
				
				) else (
					setUserProp Objs[i] "has_physics_layer" false
				)
				
				if PhysicsMaterialText.text == "" then
				(
					setUserProp Objs[i] "physics_material" "undefined"
				) else (
					setUserProp Objs[i] "physics_material" PhysicsMaterial.text
				)
				
				if LogicGenerateBox.checked then
				(
					setUserProp Objs[i] "logic_generate" true
					
					case LogicGenerateList.items[LogicGenerateList.selection] of
					(
						"stairs": setUserProp Objs[i] "logic_generate_type" "stairs"
					)	
				
				) else (
					setUserProp Objs[i] "logic_generate" false
				)
				
				if CinematicBox.checked then
				(
					setUserProp Objs[i] "cinematic_object" true
				)
				else
				(
					setUserProp Objs[i] "cinematic_object" false
				)
			) else if(SuperClassOf Objs[i] == light) then
			(
				if LightExportBox.checked then
				(
					setUserProp Objs[i] "light_export" true
				) else (
					setUserProp Objs[i] "light_export" false
				)
				if LightEnabledBox.checked then
				(
					setUserProp Objs[i] "light_enabled" true
				) else (
					setUserProp Objs[i] "light_enabled" false
				)
				if ShadowmapExportBox.checked then
				(
					setUserProp Objs[i] "shadowmap_export" true
				) else (
					setUserProp Objs[i] "shadowmap_export" false
				)
				case ShadowmapLayerList.items[ShadowmapLayerList.selection] of
					(
						"solid": setUserProp Objs[i] "shadowmap_layer" "solid"
						"alpha_objects": setUserProp Objs[i] "shadowmap_layer" "alpha_objects"
						"alpha_blend_objects": setUserProp Objs[i] "shadowmap_layer" "alpha_blend_objects"
						"particles": setUserProp Objs[i] "shadowmap_layer" "particles"
					)
				if ShadowmapWText.text == "" then
				(
					setUserProp Objs[i] "shadowmap_w" (1024 as string)
				) else (
					setUserProp Objs[i] "shadowmap_w" ShadowmapWText.text as string
				)
				if ShadowmapHText.text == "" then
				(
					setUserProp Objs[i] "shadowmap_h" (1024 as string)
				) else (
					setUserProp Objs[i] "shadowmap_h" ShadowmapHText.text as string
				)
			) else if(SuperClassOf Objs[i] == camera) then
			(
				if CameraExportBox.checked then
				(
					setUserProp Objs[i] "camera_export" true
					case CameraExportList.items[CameraExportList.selection] of
					(
						"fps": setUserProp Objs[i] "camera_export_type" "fps"
						"spherical": setUserProp Objs[i] "camera_export_type" "spherical"
						"cycle": setUserProp Objs[i] "camera_export_type" "cycle"
						"reverse": setUserProp Objs[i] "camera_export_type" "reverse"
					)
				) else (
					setUserProp Objs[i] "camera_export" false
				)
			)
		)
	)
	
	on RenderExportBox changed false do
	(
		toggleRender false
	)
	
	on RenderExportBox changed true do
	(
		toggleRender true
	)
	
	on PhysicsGenerateBox changed false do
	(
		togglePhysics false
	)
	
	on PhysicsGenerateBox changed true do
	(
		togglePhysics true
	)
	
	on LightExportBox changed false do
	(
		toggleLights false
	)
	
	on LightExportBox changed  true do
	(
		toggleLights  true
	)
	
	on ShadowmapExportBox changed  false do
	(
		toggleShadowmap  false
	)
	
	on ShadowmapExportBox changed  true do
	(
		toggleShadowmap  true
	)
	
	on CameraExportBox changed false do
	(
		toggleCameras false
	)
	
	on CameraExportBox changed  true do
	(
		toggleCameras  true
	)
	
	on GetButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona un objeto de la escena" title:"ERROR"
		) else (
			Obj = $
			GetProperties Obj
		)
	)
	
	on GetLightButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona una luz de la escena" title:"ERROR"
		) else (
			Obj = $
			GetProperties Obj
		)
	)
	
	on GetCameraButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona una cámara de la escena" title:"ERROR"
		) else (
			Obj = $
			GetProperties Obj
		)
	)
	
	on SetButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona un objeto de la escena" title:"ERROR"
			
		) else (
		
			if Obj == $ then
			(
				setProperties Obj
				
			) else (
				
				if queryBox "Hacer Set a un objeto diferente al del Get?" title:"ATENCIÓN!" beep:true then
				(
					Obj = $
					setProperties Obj
				) else (
					Obj = $
					GetProperties Obj
				)
			)
		)
	)
	
	on SetLightButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona una luz de la escena" title:"ERROR"
			
		) else (
		
			if Obj == $ then
			(
				setProperties Obj
				
			) else (
				
				if queryBox "Hacer Set a una luz diferente al del Get?" title:"ATENCIÓN!" beep:true then
				(
					Obj = $
					setProperties Obj
				) else (
					Obj = $
					GetProperties Obj
				)
			)
		)
	)
	
	on SetCameraButton pressed do
	(
		if $ == undefined then
		(
			messageBox "Selecciona una cámara de la escena" title:"ERROR"
			
		) else (
		
			if Obj == $ then
			(
				setProperties Obj
				
			) else (
				
				if queryBox "Hacer Set a una cámara diferente al del Get?" title:"ATENCIÓN!" beep:true then
				(
					Obj = $
					setProperties Obj
				) else (
					Obj = $
					GetProperties Obj
				)
			)
		)
	)
	
	on UABSetProperties open do
	(
		swapMode $
	)
	
	on UABSetProperties close do
	(
		callbacks.removeScripts #selectionSetChanged id:#swapMode
	)
)
createDialog UABSetProperties
