dofile("Data\\Lua\\Player\\PlayerGlobals.lua")
dofile("Data\\Lua\\Player\\PlayerStateMachine.lua")

function InitPlayer()
	utils_log("Init player")
	InitGlobals()
	local UABEngine = CUABEngine.get_instance()
	
	local l_Box = g_Player:get_renderable_object()
	local l_Component = l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "FnOnDebugRender")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
	
	setPlayerStateMachine()
	PlayerStateMachine:start()
end

function FnOnCreateController (_owner)
	local UABEngine = CUABEngine.get_instance()
	g_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
	g_PhysXManager:create_character_controller(g_Player.name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
	
end

function FnOnUpdateController (_owner, _ElapsedTime)
	local args = {}
	args["owner"] = _owner
	PlayerStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderController(_owner, _rm)

end

function FnOnDebugRender(_owner, _rm)
	--g_PhysXManager:render("player", _rm)
end
