dofile("Data\\Lua\\Videogame\\EnemyStateMachine.lua")

function InitEnemyMove()

	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Guardia")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponentEnemy")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponentEnemy", l_Box, "FnOnCreateEnemy","FnOnDestroyEnemy", "FnOnUpdateEnemy", "FnOnRenderEnemy", "")
		l_Box:get_component_manager():add_resource("ScriptedComponentEnemy", l_Component)
	end
	setEnemyStateMachine()
	EnemyStateMachine:start()
	
end

function FnOnCreateEnemy (_owner)
	local UABEngine = CUABEngine.get_instance()
	local l_physXManager = UABEngine:get_physX_manager()
	l_physXManager:register_material("enemyMaterial", 0.5,0.5,1)
	
	l_physXManager:create_character_controller("enemy", 1.5, 1, 0.5, _owner:get_position(),"enemyMaterial", 1)
end

function FnOnDestroyEnemy ()
	
end

function FnOnUpdateEnemy (_owner, _ElapsedTime)
	
	local args = {}
	args["owner"] = _owner
	args["enemy_name"] = "enemy"
	args["velocity"] = 10
	EnemyStateMachine:update(args,_ElapsedTime)
	
end

function FnOnRenderEnemy(_owner, _rm)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	--l_physXManager:render("enemy", _rm)
	--utils_log("render")
end