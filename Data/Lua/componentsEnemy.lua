dofile("Data\\Lua\\state_machine.lua")

EnemyStateMachine = StateMachine.create()

function setEnemyStateMAchine()

	idle_state = State.create(state1fn)
	idle_state:add_condition(state1cond1,1)
	
	state2 = State.create(state2fn)
	state2:add_condition(state2cond1,0)
	
	EnemyStateMachine:add_state(idle_state)
	EnemyStateMachine:add_state(state2)
end


function state1cond1()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	return (playerPos.x>20 or playerPos.x<-20 or playerPos.z>20 or playerPos.z<-20)
end

function state1fn(args,_ElapsedTime)

	
	local _owner = args[0]
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	
	local enemyPos = l_physXManager:get_character_controler_pos("enemy");

	local move = playerPos - enemyPos;
	l_physXManager:character_controller_move("enemy", move*0.5, _ElapsedTime);
	local l_PosCharacterController = l_physXManager:get_character_controler_pos("enemy")
	_owner:set_position(l_PosCharacterController)	

	local dir = Vect3f(math.cos(_owner:get_yaw()),0,-math.sin(_owner:get_yaw()));
	--utils_log(type(enemyPos))
	--utils_log(type(move))
	local yaw = _owner:get_yaw()
	local dot_result = dir.x*move.x + dir.y*move.y + dir.z*move.z
	--local cross_result = Vect3f(dir.y*move.z - dir.z* move.y, dir.z*move.x-dir.x*move.z, dir.x*move.y-dir.y*move.x)
	local y_cross = dir.z*move.x-dir.x*move.z
	--if y_cross >= 0 then
		_owner:set_yaw(yaw -dot_result*_ElapsedTime)
	--else
		--_owner:set_yaw(yaw + dot_result*_ElapsedTime)
	--end

--	_owner:set_yaw(
end

function state2cond1()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	return (playerPos.x<20 and playerPos.x>-20 and playerPos.z<20 and playerPos.z>-20)
	
end
function state2fn(args,elapsedTime)
end

function InitEnemyMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Int")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponentEnemy")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponentEnemy", l_Box, "FnOnCreateEnemy","FnOnDestroyEnemy", "FnOnUpdateEnemy", "FnOnRenderEnemy", "")
		l_Box:get_component_manager():add_resource("ScriptedComponentEnemy", l_Component)
	end
	setEnemyStateMAchine()
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
	args[0] = _owner
	EnemyStateMachine:update(args,_ElapsedTime)
	
end

function FnOnRenderEnemy(_owner, _rm)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	--l_physXManager:render("enemy", _rm)
	--utils_log("render")
end