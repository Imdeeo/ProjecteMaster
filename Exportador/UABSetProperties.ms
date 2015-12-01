Obj = undefined

rollout UABSetProperties "UABSetProperties" width:264 height:344
(
	GroupBox RenderGroup "Render" pos:[8,8] width:248 height:120
	GroupBox PhysicsGroup "Physics" pos:[8,136] width:248 height:80
	GroupBox LogicGroup "Logic" pos:[8,232] width:248 height:48
	checkbox RenderExportBox "Export" pos:[16,24] width:88 height:16
	checkbox RenderVisibleBox "Visible" pos:[16,48] width:88 height:16 enabled: false
	checkbox RenderLayerBox "Layer" pos:[16,72] width:88 height:16 enabled: false
	label AnimModelLabel "Anim. Model" pos:[16,104] width:88 height:16
	dropdownList RenderExportList "" pos:[112,24] width:136 height:21 items:#("instance_mesh", "animated_instance_mesh") enabled: false
	dropdownList RenderLayerList "" pos:[112,64] width:136 height:21 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles") enabled: false
	checkbox PhysicsGenerateBox "Generate" pos:[16,152] width:88 height:16
	checkbox PhysicsLayerBox "Layer" pos:[16,184] width:88 height:16 enabled: false
	dropdownList PhysicsGenerateList "" pos:[112,152] width:136 height:21 items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape") enabled: false
	dropdownList PhysicsLayerList "" pos:[112,184] width:136 height:21 items:#("logic") enabled: false
	checkbox LogicGenerateBox "Generate" pos:[16,248] width:88 height:16
	dropdownList LogicGenerateList "" pos:[112,248] width:136 height:21 items:#("stairs")
	button GetButton "Get" pos:[16,296] width:112 height:32
	button SetButton "Set" pos:[136,296] width:112 height:32
	edittext AnimModelText "" pos:[112,96] width:136 height:24 text:"undefined" enabled: false
	
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
	
	fn initUserProps Obj =
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
	)
	
	fn GetProperties Obj =
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
	)
	
	fn SetProperties Obj =
	(
		if RenderExportBox.checked then
		(
			setUserProp Obj "render_export" true
			
			case RenderExportList.items[RenderExportList.selection] of
			(
				"instance_mesh": setUserProp Obj "render_export_type" "instance_mesh"
				"animated_instance_mesh": setUserProp $ "render_export_type" "animated_instance_mesh"
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
	
)
createDialog UABSetProperties