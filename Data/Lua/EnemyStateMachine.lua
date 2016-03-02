dofile("Data\\Lua\\state_machine.lua")

EnemyStateMachine = StateMachine.create()

function setEnemyStateMAchine()

	chase_state = State.create(state1fn)
	chase_state:add_condition(state1cond1,"wait")
	chase_state:add_do_first_function(chase_do_first)
	chase_state:add_do_end_function(chase_do_end)
	
	wait_state = State.create(state2fn)
	wait_state:add_condition(state2cond1,"chase")
	wait_state:add_do_first_function(wait_do_first)
	wait_state:add_do_end_function(wait_do_end)
	
	EnemyStateMachine:add_state("chase",chase_state)
	EnemyStateMachine:add_state("wait",wait_state)
end

function chase_do_first(args)
	utils_log("Hummer: I'll get you")
end
function chase_do_end(args)
	utils_log("Hummer: Where the fuck are you?")
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

function wait_do_first(args)
	utils_log("Car: Uff! I escaped again!")
end
function wait_do_end(args)
	utils_log("Car: Let's go again! JERONIMOOOOOOOOOOOOOOOOOOOOOOO!")
end
function state2cond1()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	return (playerPos.x<20 and playerPos.x>-20 and playerPos.z<20 and playerPos.z>-20)
	
end

function state2fn(args,elapsedTime)
end
