Obj = undefined
callbacks.removeScripts #selectionSetChanged id:#swapMode
callbacks.addScript #selectionSetChanged "UABSetProperties.swapMode $" id:#swapMode persistent:false
	
rollout UABSetProperties "UABSetProperties" width:280 height:370
(
	GroupBox grp1 "Geometry" pos:[8,8] width:265 height:304
		label GeometryLabel "Nothing selected" pos:[24,32] width:232 height:24
		GroupBox RenderGroup "Render" pos:[16,56] width:248 height:120
			checkbox RenderExportBox "Export" pos:[24,72] width:88 height:16
			dropdownList RenderExportList "" pos:[120,72] width:136 height:21 enabled:false items:#("instance_mesh", "animated_instance_mesh")
			checkbox RenderVisibleBox "Visible" pos:[24,96] width:88 height:16 enabled:false
			checkbox RenderLayerBox "Layer" pos:[24,120] width:88 height:16 enabled:false
			dropdownList RenderLayerList "" pos:[120,112] width:136 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
			label AnimModelLabel "Anim. Model" pos:[24,152] width:88 height:16
			edittext AnimModelText "undefined" pos:[120,144] width:136 height:24 enabled:false
		GroupBox PhysicsGroup "Physics" pos:[16,176] width:248 height:80
			checkbox PhysicsGenerateBox "Generate" pos:[24,200] width:88 height:16
			dropdownList PhysicsGenerateList "" pos:[120,200] width:136 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
			checkbox PhysicsLayerBox "Layer" pos:[24,232] width:88 height:16 enabled:false
			dropdownList PhysicsLayerList "" pos:[120,232] width:136 height:21 enabled:false items:#("logic")
		GroupBox LogicGroup "Logic" pos:[16,256] width:248 height:48
			checkbox LogicGenerateBox "Generate" pos:[24,272] width:88 height:16
			dropdownList LogicGenerateList "" pos:[120,272] width:136 height:21 items:#("stairs")
		button GetButton "Get" pos:[24,320] width:112 height:32
		button SetButton "Set" pos:[144,320] width:112 height:32
	GroupBox grp2 "Lights" pos:[8,8] width:265 height:88 visible:false
		label LightLabel "Nothing selected" pos:[24,32] width:232 height:24 visible:false
		checkbox LightExportBox "Export" pos:[24,64] width:88 height:16 visible:false
		button GetLightButton "Get" pos:[24,104] width:112 height:32 visible:false
		button SetLightButton "Set" pos:[144,104] width:112 height:32 visible:false
	GroupBox grp3 "Inactive" pos:[8,8] width:265 height:61 visible:false
		label UnsupportedLabel "Current selection is not supported." pos:[24,32] width:232 height:24 visible:false
	
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
		AnimModelLabel.visible = bool
		AnimModelText.visible = bool
		PhysicsGroup.visible = bool
		PhysicsGenerateBox.visible = bool
		PhysicsGenerateList.visible = bool
		PhysicsLayerBox.visible = bool
		PhysicsLayerList.visible = bool
		LogicGroup.visible = bool
		LogicGenerateBox.visible = bool
		LogicGenerateList.visible = bool
		GetButton.visible = bool
		SetButton.visible = bool
	)
	
	fn toggleLightsGroup bool =
	(
		grp2.visible = bool
		LightLabel.visible = bool
		LightExportBox.visible = bool
		GetLightButton.visible = bool
		SetLightButton.visible = bool
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
	)
	
	fn togglePhysics bool =
	(
		PhysicsGenerateList.enabled = bool
		PhysicsLayerBox.enabled =  bool
		PhysicsLayerList.enabled =  bool
	)
	
	fn loadUnsupportedSelection Obj =
	(
		toggleLightsGroup false
		toggleGeometryGroup false
		toggleUnsupportedGroup true
		UABSetProperties.height = 80
		if (Obj == undefined) then
		(
			UnsupportedLabel.text = "Nothing selected."
		) else (
			UnsupportedLabel.text = ""+(ClassOf Obj) as string+" is not a supported object type."
		)
	)
	
	fn loadLightMode Obj =
	(
		if (ClassOf Obj == Omnilight) or
		(ClassOf Obj == targetSpot) or
		(ClassOf Obj == freeSpot) or
		(ClassOf Obj == Directionallight) or
		(ClassOf Obj == TargetDirectionallight)
		then (
			toggleLightsGroup true
			toggleGeometryGroup false
			toggleUnsupportedGroup false
			LightLabel.text = Obj.name+" ("+(ClassOf Obj) as string+")"
			UABSetProperties.height = 150
		) else (
			loadUnsupportedSelection Obj
		)
	)
	
	fn loadGeometryMode Obj =
	(
		if (findString Obj.name ".Target" == undefined) then
		(
			toggleLightsGroup false
			toggleGeometryGroup true
			toggleUnsupportedGroup false
			GeometryLabel.text = Obj.name+" ("+(ClassOf Obj) as string+")"
			UABSetProperties.height = 370
		)
		else
		(
			loadUnsupportedSelection Obj
		)
	)
		
	fn swapMode Obj =
	(
		case SuperClassOf Obj of
		(
			light: loadLightMode Obj
			GeometryClass:  loadGeometryMode Obj
			default: loadUnsupportedSelection Obj
		)
	)
	
	fn initUserProps Obj =
	(
		if(SuperClassOf Obj == GeometryClass) then
		(
			setUserProp Obj "render_export" false
			setUserProp Obj "render_export_type" "instance_mesh"
			setUserProp Obj "render_visible" false
			setUserProp Obj "has_render_layer" false
			setUserProp Obj "render_layer" "solid"
			setUserProp Obj "anim_model_core" "undefined"
			setUserProp Obj "physics_generate" false
			setUserProp Obj "physics_generate_type" "triangle_mesh"
			setUserProp Obj "has_physics_layer" false
			setUserProp Obj "physics_layer" "logic"
			setUserProp Obj "logic_generate" false
			setUserProp Obj "logic_generate_type" "stairs"
		) else (
			setUserProp Obj "light_export" false
		)
	)
	
	fn GetProperties Obj =
	(
		if(SuperClassOf Obj == GeometryClass) then
		(
			if getUserProp Obj "render_export" == undefined then
			(
				initUserProps Obj
			)
			RenderExportBox.checked = getUserProp Obj "render_export"
			if RenderExportBox.checked == true then
			(
				toggleRender true
				
				case getUserProp Obj "render_export_type" of
					(
						"instance_mesh": RenderExportList.selection = 1
						"animated_instance_mesh": RenderExportList.selection = 2
					)
			
				RenderVisibleBox.checked = getUserProp Obj "render_visible"
			
				RenderLayerBox.checked = getUserProp Obj "has_render_layer"
				case getUserProp Obj "render_layer" of
					(
						"solid": RenderLayerList.selection = 1
						"alpha_objects": RenderLayerList.selection = 2
						"alpha_blend_objects": RenderLayerList.selection = 3
						"particles": RenderLayerList.selection = 4
					)
			
				AnimModelText.text = getUserProp Obj "anim_model_core"
			
			) else (
				
				toggleRender false
				RenderVisibleBox.checked = false
				RenderLayerBox.checked = false
			)
			
			PhysicsGenerateBox.checked = getUserProp Obj "physics_generate"
			if PhysicsGenerateBox.checked == true then
			(
				togglePhysics true
				case getUserProp Obj "physics_generate_type" of
					(
						"triangle_mesh": PhysicsGenerateList.selection = 1
						"convex_shape": PhysicsGenerateList.selection = 2
						"box_shape": PhysicsGenerateList.selection = 3
						"sphere_shape": PhysicsGenerateList.selection = 4
						"plane_shape": PhysicsGenerateList.selection = 5
						"capsule_shape": PhysicsGenerateList.selection = 6
					)
			
				PhysicsLayerBox.checked = getUserProp Obj "has_physics_layer"
				case getUserProp Obj "physics_layer" of
					(
						"logic": PhysicsLayerList.selection = 1
					)
			) else (
				
				togglePhysics false
				PhysicsLayerBox.checked = false
			)
			
			LogicGenerateBox.checked = getUserProp $ "logic_generate"
			case getUserProp Obj "logic_generate_type" of
				(
					"stairs": LogicGenerateList.selection = 1
				)
		) else (
			if getUserProp Obj "light_export" == undefined then
			(
				initUserProps Obj
			)
			LightExportBox.checked = getUserProp Obj "light_export"
		)
	)
	
	fn SetProperties Obj =
	(
		if(SuperClassOf Obj == GeometryClass) then
		(
			if RenderExportBox.checked then
			(
				setUserProp Obj "render_export" true
				
				case RenderExportList.items[RenderExportList.selection] of
				(
					"instance_mesh": setUserProp Obj "render_export_type" "instance_mesh"
					"animated_instance_mesh": setUserProp Obj "render_export_type" "animated_instance_mesh"
				)	
			
			) else (
				setUserProp Obj "render_export" false
			)
			
			if RenderVisibleBox.checked then
			(
				setUserProp Obj "render_visible" true
			) else (
				setUserProp Obj "render_visible" false
			)
			
			if RenderLayerBox.checked then
			(
				setUserProp Obj "has_render_layer" true
				
				case RenderLayerList.items[RenderLayerList.selection] of
				(
					"solid": setUserProp Obj "render_layer" "solid"
					"alpha_objects": setUserProp Obj "render_layer" "alpha_objects"
					"alpha_blend_objects": setUserProp Obj "render_layer" "alpha_blend_objects"
					"particles": setUserProp Obj "render_layer" "particles"
				)	
				
			) else (
				setUserProp Obj "has_render_layer" false
			)
			
			if AnimModelText.text == "" then
			(
				--messageBox "Introduce el core para el modelo animado" title:"ERROR"
				setUserProp Obj "anim_model_core" "undefined"
			) else (
				setUserProp Obj "anim_model_core" AnimModelText.text
			)
			
			if PhysicsGenerateBox.checked then
			(
				setUserProp Obj "physics_generate" true
				
				case PhysicsGenerateList.items[PhysicsGenerateList.selection] of
				(
					"triangle_mesh": setUserProp Obj "physics_generate_type" "triangle_mesh"
					"convex_shape": setUserProp Obj "physics_generate_type" "convex_shape"
					"box_shape": setUserProp Obj "physics_generate_type" "box_shape"
					"sphere_shape": setUserProp Obj "physics_generate_type" "sphere_shape"
					"plane_shape": setUserProp Obj "physics_generate_type" "plane_shape"
					"capsule_shape": setUserProp Obj "physics_generate_type" "capsule_shape"
				)	
			
			) else (
				setUserProp Obj "physics_generate" false
			)
			
			if PhysicsLayerBox.checked then
			(
				setUserProp Obj "has_physics_layer" true
				
				case PhysicsLayerList.items[PhysicsLayerList.selection] of
				(
					"logic": setUserProp Obj "physics_layer" "logic"
				)	
			
			) else (
				setUserProp Obj "has_physics_layer" false
			)
			
			if LogicGenerateBox.checked then
			(
				setUserProp Obj "logic_generate" true
				
				case LogicGenerateList.items[LogicGenerateList.selection] of
				(
					"stairs": setUserProp Obj "logic_generate_type" "stairs"
				)	
			
			) else (
				setUserProp Obj "logic_generate" false
			)
		) else (
			if LightExportBox.checked then
			(
				setUserProp Obj "light_export" true
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
	
	on PhysicsGeneratebox changed false do
	(
		togglePhysics false
	)
	
	on PhysicsGeneratebox changed true do
	(
		togglePhysics true
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







