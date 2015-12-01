rollout UABSetProperties "UABSetProperties" width:264 height:344
(
	GroupBox RenderGroup "Render" pos:[8,8] width:248 height:120
	GroupBox PhysicsGroup "Physics" pos:[8,136] width:248 height:80
	GroupBox LogicGroup "Logic" pos:[8,232] width:248 height:48
	checkbox RenderExportBox "Export" pos:[16,24] width:88 height:16
	checkbox RenderVisibleBox "Visible" pos:[16,48] width:88 height:16
	checkbox RenderLayerBox "Layer" pos:[16,72] width:88 height:16
	label AnimModelLabel "Anim. Model" pos:[16,104] width:88 height:16
	dropdownList RenderExportList "" pos:[112,24] width:136 height:21
	dropdownList RenderLayerList "" pos:[112,64] width:136 height:21
	checkbox PhysicsGenerateBox "Generate" pos:[16,152] width:88 height:16
	checkbox PhysicsLayerBox "Layer" pos:[16,184] width:88 height:16
	dropdownList PhysicsGenerateList "" pos:[112,152] width:136 height:21
	dropdownList PhysicsLayerList "" pos:[112,184] width:136 height:21
	checkbox LogicGenerateBox "Generate" pos:[16,248] width:88 height:16
	dropdownList LogicGenerateList "" pos:[112,248] width:136 height:21
	button GetButton "Get" pos:[16,296] width:112 height:32
	button SetButton "Set" pos:[136,296] width:112 height:32
	edittext AnimModelText "" pos:[112,96] width:136 height:24
)
createDialog UABSetProperties

-- Si haces set en uno deiferente al del get, que diga "EH que es diferente, confirmar? s/n"