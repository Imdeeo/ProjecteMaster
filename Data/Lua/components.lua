function InitComponents()
	InitBoxMove()
end

function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	--local l_Component=l_Box:GetComponentManager():GetResource("ScriptedCharacterController")
	--if l_Component==nil then
	--	local l_Component=CreateScriptedComponent("ScriptedCharacterController", l_Box, "FnOnCreateWitcher","FnOnDestroyWitcher", "FnOnUpdateWitcher", "", "")
	--	l_Box:AddComponent(l_Component)
	--end
end