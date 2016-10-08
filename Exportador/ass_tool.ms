fileIn "animated_camera.ms"
fileIn "animated_end_position.ms"
fileIn "camera.ms"
fileIn "character.ms"
fileIn "cinematic.ms"
fileIn "export.ms"
fileIn "light.ms"
fileIn "navmesh.ms"
fileIn "renderable_object.ms"
fileIn "static_mesh.ms"

Obj = undefined
callbacks.removeScripts #selectionSetChanged id:#swapMode
callbacks.addScript #selectionSetChanged "ASSTool.swapMode $" id:#swapMode persistent:false

rollout ASSTool "A Simple Song Export Tool" width:720 height:410
(
	GroupBox div "" pos:[139,0] width:4 height:380 visible:true
		button ExportPathButton "Export Path" pos:[14,24] width:112 height:32 visible:true
		label ExportPathLabel "C:/export_path_example/ psyduck" pos:[14,64] width:112 height:24 visible:true
		dropDownList LevelList "" pos:[14,94] width:112 height:21
		button ExportNavButton "Navmesh" pos:[14,124] width:112 height:32 visible:true
		button ExportXMLButton "XML" pos:[14,164] width:112 height:32 visible:true
		button ExportMeshButton "Mesh" pos:[14,204] width:112 height:32 visible:true
		checkbox ExportMaterialCheck "Materials" pos:[40,244] width:60 height:16 checked:true
		checkbox ExportMeshCheck "Meshes" pos:[40,264] width:56 height:16
		checkbox ExportTexturesCheck "Textures" pos:[40,284] width:64 height:16
		button ExportSceneButton "Scene" pos:[14,304] width:112 height:32 visible:true
		label SelectionLabel "Nothing selected" pos:[160,32] width:416 height:24
	GroupBox grp1 "Geometry" pos:[152,8] width:432 height:358
		GroupBox RenderGroup "Render" pos:[160,56] width:204 height:168
			checkbox RenderExportBox "Export" pos:[168,72] width:88 height:16
			checkbox RenderVisibleBox "Visible" pos:[168,96] width:88 height:16 enabled:false
			label RenderLayerLabel "Layer" pos:[188,120] width:88 height:16 enabled:false
			checkbox CinematicBox "Cinematic" pos:[168,144] width:88 height:16 enabled:false
			checkbox CinematicCalBox "Cal3D" pos:[264,144] width:88 height:16 enabled:false
			dropdownList RenderLayerList "" pos:[264,117] width:88 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "skybox", "triggers")
			checkbox AnimatedBox "Animated Model" pos:[168,194] width:96 height:16 enabled:false 
			edittext AnimModelText "" pos:[264,192] width:88 height:24 enabled:false
			label CinematicNameLabel "Cinematic Name" pos:[186,170] width:80 height:16 enabled:false
			edittext CinematicNameText "" pos:[264,166] width:88 height:24 enabled:false
		GroupBox PhysicsGroup "Physics" pos:[372,56] width:204 height:304
			checkbox PhysicsGenerateBox "Generate" pos:[380,80] width:80 height:16
			dropdownList PhysicsGenerateList "" pos:[464,78] width:98 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "box_trigger", "sphere_shape", "plane_shape", "capsule_shape")
			label PhysicsGroupLabel "Group" pos:[380,104] width:88 height:16 enabled:false
			dropdownList PhysicsLayerList "" pos:[464,102] width:98 height:21 enabled:false items:#("plane", "objects", "triggers", "player")
			edittext PhysicsMaterialText "Material" pos:[380,128] width:180 height:24 enabled:false
			GroupBox TriggerGroup "Trigger" pos:[380,160] width:188 height:108 enabled:false 
				edittext PhysicsTriggerEnter "Enter" pos:[388,180] width:172 height:24 enabled:false 
				edittext PhysicsTriggerExit "Exit  " pos:[388,208] width:172 height:24 enabled:false 
				edittext PhysicsTriggerStay "Stay " pos:[388,236] width:172 height:24 enabled:false 
			GroupBox PlaneGroup "Plane" pos:[380,276] width:188 height:76 enabled:false
				spinner PlaneOffset "Offset:" pos:[418,294] width:64 range:[-1000,1000,0] type:#float scale:0.01 enabled:false
				label PlaneNormal "Normal:" pos:[388,326] width:36 height:16 enabled:false 
				spinner PlaneX pos:[430,326] width:42 range:[-1000,1000,0] type:#float scale:0.01 enabled:false
				spinner PlaneY pos:[475,326] width:42 range:[-1000,1000,0] type:#float scale:0.01 enabled:false
				spinner PlaneZ pos:[520,326] width:42 range:[-1000,1000,0] type:#float scale:0.01 enabled:false
				label LabelX "x" pos:[445,310] width:8 height:12 enabled:false
				label LabelY "y" pos:[488,310] width:8 height:12 enabled:false
				label LabelZ "z" pos:[535,310] width:8 height:12 enabled:false
		GroupBox LogicGroup "Logic" pos:[160,232] width:204 height:48
			checkbox LogicGenerateBox "Generate" pos:[168,256] width:88 height:16
			dropdownList LogicGenerateList "" pos:[264,254] width:88 height:21 items:#("stairs")
		button AnimationEndButton "Animation End" pos:[166,304] width:192 height:32
		button GetButton "Get" pos:[592,144] width:112 height:32
		button SetButton "Set" pos:[592,184] width:112 height:32
	GroupBox grp2 "Lights" pos:[152,8] width:432 height:358 visible:false
			label LightLabel "Nothing selected" pos:[168,42] width:232 height:24 visible:false
			checkbox LightExportBox "Export" pos:[168,80] width:88 height:16 visible:false
			checkbox LightEnabledBox "Enabled" pos:[168,100] width:88 height:16 visible:false enabled:false
		GroupBox ShadowmapGroup "Shadowmap" pos:[280,60] width:248 height:80 visible:false
			checkbox ShadowmapExportBox "Export" pos:[288,82] width:88 height:16 visible:false
			dropdownList ShadowmapLayerList "" pos:[384,82] width:136 height:21 visible:false enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
			label ShadowmapWHLabel "Width and height" pos:[288,109] width:88 height:16 visible:false
			edittext ShadowmapWText pos:[381,107] width:40 height:20 visible:false enabled:false text: "1024"
			edittext ShadowmapHText pos:[421,107] width:40 height:20 visible:false enabled:false text: "1024"
		button GetLightButton "Get" pos:[592,144] width:112 height:32 visible:false
		button SetLightButton "Set" pos:[592,184] width:112 height:32 visible:false
	GroupBox grp3 "Inactive" pos:[152,8] width:432 height:358 visible:false
	GroupBox grp4 "Cameras" pos:[152,8] width:432 height:358 visible:false
		label CameraLabel "Nothing selected" pos:[168,32] width:232 height:24 visible:false
		checkbox CameraExportBox "Export" pos:[168,64] width:88 height:16 visible:false
		dropdownList CameraExportList "" pos:[268,62] width:136 height:21 visible:false enabled:false items:#("fps", "spherical", "cycle", "reverse")
		button ExportAnimatedCamera "Animated Camera" pos:[168,104] width:224 height:32 visible:false
		button GetCameraButton "Get" pos:[592,144] width:112 height:32 visible:false
		button SetCameraButton "Set" pos:[592,184] width:112 height:32 visible:false
	progressBar prog pos:[1,396] width:718 height:8 color:green
	label progLabel "Progress bar" pos:[8,380] width:680 height:16
	
	fn toggleGeometryGroup bool =
	(
		grp1.visible = bool
		RenderGroup.visible = bool
		RenderExportBox.visible = bool
		RenderVisibleBox.visible = bool
		RenderLayerLabel.visible = bool
		RenderLayerList.visible = bool
		CinematicBox.visible = bool
		CinematicCalBox.visible = bool
		AnimatedBox.visible = bool
		AnimModelText.visible = bool
		CinematicNameLabel.visible = bool
		CinematicNameText.visible = bool
		PhysicsGroup.visible = bool
		PhysicsGenerateBox.visible = bool
		PhysicsGenerateList.visible = bool
		PhysicsGroupLabel.visible = bool
		PhysicsLayerList.visible = bool
		PhysicsMaterialText.visible = bool
		LogicGroup.visible = bool
		LogicGenerateBox.visible = bool
		LogicGenerateList.visible = bool
		GetButton.visible = bool
		SetButton.visible = bool
		TriggerGroup.visible = bool
		PhysicsTriggerEnter.visible = bool
		PhysicsTriggerExit.visible = bool
		PhysicsTriggerStay.visible = bool
		PlaneGroup.visible = bool
		PlaneOffset.visible = bool
		PlaneNormal.visible = bool
		PlaneX.visible = bool
		PlaneY.visible = bool
		PlaneZ.visible = bool
		LabelX.visible = bool
		LabelY.visible = bool
		LabelZ.visible = bool
		AnimationEndButton.visible = bool
	)
	
	fn toggleLightsGroup bool =
	(
		grp2.visible = bool
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
		CameraExportBox.visible = bool
		CameraExportList.visible = bool
		ExportAnimatedCamera.visible = bool
		GetCameraButton.visible = bool
		SetCameraButton.visible = bool
	)
	
	fn toggleUnsupportedGroup bool =
	(
		grp3.visible = bool
	)
	
	fn toggleCinematics bool =
	(
		CinematicCalBox.enabled = bool
		CinematicNameLabel.enabled = bool
		CinematicNameText.enabled = bool
	)
	
	fn toggleRender bool =
	(
		RenderVisibleBox.enabled = bool
		RenderLayerLabel.enabled = bool
		RenderLayerList.enabled = bool
		AnimatedBox.enabled = bool
		CinematicBox.enabled = bool
		CinematicCalBox.enabled = bool
		if AnimatedBox.checked == bool then
		(
			AnimModelText.enabled = bool
		) else (
			AnimModelText.enabled = false
		)
		if CinematicBox.checked == bool then
		(
			toggleCinematics bool
		) else (
			toggleCinematics false
		)
	)
	
	fn togglePhysics bool =
	(
		PhysicsGenerateList.enabled = bool
		PhysicsGroupLabel.enabled =  bool
		PhysicsLayerList.enabled =  bool
		PhysicsMaterialText.enabled = bool
	)
	
	fn toggleTriggers bool =
	(
		TriggerGroup.enabled = bool
		PhysicsTriggerEnter.enabled = bool
		PhysicsTriggerExit.enabled = bool
		PhysicsTriggerStay.enabled = bool
	)
	
	fn togglePlane bool =
	(
		PlaneGroup.enabled = bool
		PlaneOffset.enabled = bool
		PlaneNormal.enabled = bool
		PlaneX.enabled = bool
		PlaneY.enabled = bool
		PlaneZ.enabled = bool
		LabelX.enabled = bool
		LabelY.enabled = bool
		LabelZ.enabled = bool
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
		if (Obj == undefined) then
		(
			SelectionLabel.text = "Nothing selected."
		) else (
			SelectionLabel.text = ""+(ClassOf Obj) as string+" is not a supported selection."
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
			SelectionLabel.text = textLabel
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
			SelectionLabel.text = textLabel
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
		SelectionLabel.text = textLabel
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
			setUserProp Obj "cinematic_name" "undefined"
			setUserProp Obj "physics_generate" false
			setUserProp Obj "physics_generate_type" "triangle_mesh"
			setUserProp Obj "has_physics_layer" false
			setUserProp Obj "physics_layer" (1 as string)
			setUserProp Obj "physics_material" ""
			setUserProp Obj "logic_generate" false
			setUserProp Obj "logic_generate_type" "stairs"
			setUserProp Obj "cinematic_object" false
			setUserProp Obj "on_trigger_enter" ("")
			setUserProp Obj "on_trigger_exit" ("")
			setUserProp Obj "on_trigger_stay" ("")
			setUserProp Obj "plane_offset" 0
			setUserProp Obj "plane_normal_x" 0
			setUserProp Obj "plane_normal_y" 1
			setUserProp Obj "plane_normal_z" 0
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
			
			if (getUserProp Objs[i] "render_export" != undefined) and (getUserProp Objs[i] "cinematic_name" == undefined) then
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
						"instance_mesh": AnimatedBox.checked = false
						"animated_instance_mesh": AnimatedBox.checked = true
					)
				
				RenderVisibleBox.checked = getUserProp Objs[1] "render_visible"
				
				case getUserProp Objs[1] "render_layer" of
				(
					"solid": RenderLayerList.selection = 1
					"alpha_objects": RenderLayerList.selection = 2
					"alpha_blend_objects": RenderLayerList.selection = 3
					"particles": RenderLayerList.selection = 4
					"skybox": RenderLayerList.selection = 5
					"triggers": RenderLayerList.selection = 6
				)
				
				if getUserProp Objs[1] "render_layer" == "triggers" then
					toggleTriggers true
				else
					toggleTriggers false
			
				AnimModelText.text = getUserProp Objs[1] "anim_model_core"
				CinematicBox.checked = getUserProp Objs[1] "cinematic_object"
				CinematicNameText.text = getUserProp Objs[1] "cinematic_name"
			) else (
				toggleRender false
				RenderVisibleBox.checked = false
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
					"box_trigger": PhysicsGenerateList.selection = 4
					"sphere_shape": PhysicsGenerateList.selection = 5
					"plane_shape": PhysicsGenerateList.selection = 6
					"capsule_shape": PhysicsGenerateList.selection = 7
				)
				
				if getUserProp Objs[1] "physics_generate_type" == "plane_shape" then
					togglePlane true
				else
					togglePlane false
				
				case getUserProp Objs[1] "physics_layer" of
				(
					"plane": PhysicsLayerList.selection = 1
					"objects": PhysicsLayerList.selection = 2
					"triggers": PhysicsLayerList.selection = 3
					"player": PhysicsLayerList.selection = 4
				)
					
				PhysicsMaterialText.text = getUserProp Objs[1] "physics_material"
				PhysicsTriggerEnter.text = getUserProp Objs[1] "on_trigger_enter"
				PhysicsTriggerExit.text = getUserProp Objs[1] "on_trigger_exit"
				PhysicsTriggerStay.text = getUserProp Objs[1] "on_trigger_stay"
				
				PlaneOffset.value = getUserProp Objs[1] "plane_offset"
				PlaneX.value = getUserProp Objs[1] "plane_normal_x"
				PlaneY.value = getUserProp Objs[1] "plane_normal_y"
				PlaneZ.value = getUserProp Objs[1] "plane_normal_z"
				
			) else (
				togglePhysics false
				PhysicsMaterialText.text = ""
				PhysicsTriggerEnter.text = ""
				PhysicsTriggerExit.text = ""
				PhysicsTriggerStay.text = ""
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
					
					if AnimatedBox.checked then
					(
						setUserProp Objs[i] "render_export_type" "animated_instance_mesh"
					) else (
						setUserProp Objs[i] "render_export_type" "instance_mesh"
					)
				
					if RenderVisibleBox.checked then
					(
						setUserProp Objs[i] "render_visible" true
					) else (
						setUserProp Objs[i] "render_visible" false
					)
					
					case RenderLayerList.items[RenderLayerList.selection] of
					(
						"solid": setUserProp Objs[i] "render_layer" "solid"
						"alpha_objects": setUserProp Objs[i] "render_layer" "alpha_objects"
						"alpha_blend_objects": setUserProp Objs[i] "render_layer" "alpha_blend_objects"
						"particles": setUserProp Objs[i] "render_layer" "particles"
						"skybox": setUserProp Objs[i] "render_layer" "skybox"
						"triggers": setUserProp Objs[i] "render_layer" "triggers"
					)
					
					if CinematicBox.checked then
					(
						setUserProp Objs[i] "cinematic_name" CinematicNameText.text
					) else (
						setUserProp Objs[i] "cinematic_name" "undefined"
					)
					
					if AnimModelText.text == "" then
					(
						setUserProp Objs[i] "anim_model_core" "undefined"
					) else (
						setUserProp Objs[i] "anim_model_core" AnimModelText.text
					)
					
				) else (
					setUserProp Objs[i] "render_export" false
				)
				
				if PhysicsGenerateBox.checked then
				(
					setUserProp Objs[i] "physics_generate" true
					
					case PhysicsGenerateList.items[PhysicsGenerateList.selection] of
					(
						"triangle_mesh": setUserProp Objs[i] "physics_generate_type" "triangle_mesh"
						"convex_shape": setUserProp Objs[i] "physics_generate_type" "convex_shape"
						"box_shape": setUserProp Objs[i] "physics_generate_type" "box_shape"
						"box_trigger": setUserProp Objs[i] "physics_generate_type" "box_trigger"
						"sphere_shape": setUserProp Objs[i] "physics_generate_type" "sphere_shape"
						"plane_shape": setUserProp Objs[i] "physics_generate_type" "plane_shape"
						"capsule_shape": setUserProp Objs[i] "physics_generate_type" "capsule_shape"
					)
					
					case PhysicsLayerList.items[PhysicsLayerList.selection] of
					(
						"plane": setUserProp Objs[i] "physics_layer" "Plane"
						"objects": setUserProp Objs[i] "physics_layer" "Objects"
						"triggers": setUserProp Objs[i] "physics_layer" "Triggers"
						"player": setUserProp Objs[i] "physics_layer" "Player"
					)
					
					if PhysicsMaterialText.text == "" then
					(
						setUserProp Objs[i] "physics_material" "undefined"
					) else (
						setUserProp Objs[i] "physics_material" PhysicsMaterialText.text
					)
					
					setUserProp Objs[i] "on_trigger_enter" PhysicsTriggerEnter.text
					setUserProp Objs[i] "on_trigger_exit" PhysicsTriggerExit.text
					setUserProp Objs[i] "on_trigger_stay" PhysicsTriggerStay.text
					
					setUserProp Objs[i] "plane_offset" PlaneOffset.value
					setUserProp Objs[i] "plane_normal_x" PlaneX.value
					setUserProp Objs[i] "plane_normal_y" PlaneY.value
					setUserProp Objs[i] "plane_normal_z" PlaneZ.value
				
				) else (
					setUserProp Objs[i] "physics_generate" false
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
		toggleTriggers false
		togglePlane false
	)
	
	on PhysicsGenerateBox changed true do
	(
		togglePhysics true
		if PhysicsGenerateList.selection == 5 then
			togglePlane true
		if RenderLayerList.selection == 6 then
			toggleTriggers true
	)
	
	on CinematicBox changed false do
	(
		toggleCinematics false
	)
	
	on CinematicBox changed  true do
	(
		toggleCinematics true
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
	
	on AnimatedBox changed true do
	(
		AnimModelText.enabled = true
	)
	
	on AnimatedBox changed false do
	(
		AnimModelText.enabled = false
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
	
	on RenderLayerList selected itm do
	(
		if itm == 6 and PhysicsGenerateBox.checked then
			toggleTriggers true
		else
			toggleTriggers false
	)
	
	on PhysicsGenerateList selected itm do
	(
		if itm == 5 and PhysicsGenerateBox.checked then
			togglePlane true
		else
			togglePlane false
	)
	
	-- Export panel
	
	fn get_Dir =
	(
		local dir = ""+ExportPathLabel.text+"\\" 
		dirExist dir
		dir += LevelList.items[LevelList.selection] + "\\"
		dirExist dir
		dir
	)
	
	fn getRawDir =
	(
		local dir = ""+ExportPathLabel.text+"\\" 
		dirExist dir
		dir
	)
	
	fn exportLights dir =
	(
		local lightsToExport = #()
		checkUserProp "light_export" &lightsToExport
		createLights dir "lights.xml" lightsToExport
	)

	fn exportCameras dir =
	(
		local camerasToExport = #()
		checkUserProp "camera_export" &camerasToExport
		createCameras dir camerasToExport
	)

	fn exportMesh dir Obj =
	(
		local Objs = #()
		
		if ClassOf Obj != ObjectSet then
		(
			if (SuperClassOf  Obj) == GeometryClass then
			(
				append Objs Obj
				createMeshFromObjects dir Objs &prog &progLabel
			)
			else
			(
				messageBox "You must select a geometry!" title:"ERROR"
			)
		)
		else
		(
			local nonGeometry = false
			for i=1 to Obj.count do
			(
				if SuperClassOf Obj[i]  == GeometryClass then
				(
					append Objs Obj[i]
				)
				else
				(
					nonGeometry = true
				)
			)
			if nonGeometry == false then
			(
				createMeshFromObjects dir Objs
			)
			else
			(
				messageBox "You have selected one or more non-geometry objects!" title:"ERROR"
			)
		)
	)

	fn exportMaterials dir textureCheck =
	(
		local materialsToExport = #()
		checkUserProp "render_export" &materialsToExport
		createMaterials dir "materials.xml" materialsToExport textureCheck
	)

	fn exportStaticMesh dir meshCheck =
	(
		local meshToExport = #()
		checkUserProp "render_export" &meshToExport
		local coresToExport = #()
		getCores meshToExport &coresToExport
		createStaticMesh dir (LevelList.items[LevelList.selection]) "static_meshes.xml" coresToExport
		if meshCheck == true then 
		(
			createMeshFromObjects dir coresToExport &prog &progLabel
		)
	)

	fn exportRenderableObject dir =
	(
		local cinematicObjectsToExport = #()
		checkUserProp "cinematic_object" &cinematicObjectsToExport
		createCinematicObjects dir "cinematic.xml" cinematicObjectsToExport CinematicCalBox.checked
		local renderableObjectsToExport = #()
		checkUserProp "render_export" &renderableObjectsToExport
		createInstanceMesh dir "renderable_objects.xml" renderableObjectsToExport
	)
	
	on ExportPathButton pressed do
	(
		local l_Path=getSavePath caption:"Select game folder" initialDir:ExportPathLabel.text
		if l_Path!=undefined then
		(
			g_Path = l_Path
			ExportPathLabel.text = l_Path--+"\\"+LevelList.items[LevelList.selection]+"\\"
		)
	)
	
	on ExportMeshButton pressed do
    (
		if $ == undefined then
		(
			messageBox "You must select a scene object" title:"ERROR"
		)
		else
		(
			undo off(
				local theWatch = PerformanceTester()
				theWatch.start()
				exportMesh (get_Dir()) $
				prog.value = 0
				progLabel.text = "Done!"
				theWatch.stop()
				theWatch.report()
			)
		)
    )
	
	on ExportXMLButton pressed do
	(
		local dir = get_Dir()
		exportLights dir
		exportCameras dir
		createCharacters dir
		exportStaticMesh dir false
		exportRenderableObject dir
		if ExportMaterialCheck.checked then
		(
			exportMaterials dir (ExportTexturesCheck.checked)
		)
	)
	
	on ExportSceneButton pressed do
	(
		--checkForSave() 
		undo off(
			local theWatch = PerformanceTester()
			theWatch.start()
			local dir = get_Dir()
			exportLights dir
			exportCameras dir
			createCharacters dir
			exportMaterials dir (ExportTexturesCheck.checked)
			exportStaticMesh dir (ExportMeshCheck.checked)
			exportRenderableObject dir
			prog.value = 0
			progLabel.text = "Done!"
			theWatch.stop()
			theWatch.report()
		)
		--resetMaxFile()
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
	
	on ExportAnimatedCamera pressed do
	(
		callbacks.removeScripts #selectionSetChanged id:#checkCam
		callbacks.addScript #selectionSetChanged "CamAnim.checkCam $" id:#checkCam persistent:false
		createDialog CamAnim
	)
	
	on AnimationEndButton pressed do
	(
		ExportAnimationEndPosition (getRawDir())
	)
	
	on ASSTool open do
	(
		ExportPathLabel.text="D:\\Data"
		local levels=#("level_1", "level_2", "level_3", "level_4", "level_5", "level_6", "level_7", "level_8")
		LevelList.items=levels
		g_Path = ExportPathLabel.text+"\\"+LevelList.items[LevelList.selection]+"\\"
		swapMode $
	)
	
	on ASSTool close do
	(
		callbacks.removeScripts #selectionSetChanged id:#swapMode
	)
)
createDialog ASSTool
