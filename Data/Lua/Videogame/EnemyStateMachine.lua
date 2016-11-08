dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Utils\\EnemyUtils.lua")

EnemyStateMachine = StateMachine.create()

function setEnemyStateMachine()

	chase_state = State.create(state1fn)
	chase_state:add_condition(state1cond1,"patrol")
	chase_state:set_do_first_function(chase_do_first)
	chase_state:set_do_end_function(chase_do_end)
	
	patrol_state = State.create(patrol_update)
	patrol_state:add_condition(state2cond1,"chase")
	patrol_state:set_do_first_function(patrol_first)
	
	--utils_log("doing these")
	
	wait_state = State.create(state2fn)
	wait_state:add_condition(state2cond1,"chase")
	wait_state:set_do_first_function(wait_do_first)
	wait_state:set_do_end_function(wait_do_end)
	
	EnemyStateMachine:add_state("chase",chase_state)
	EnemyStateMachine:add_state("wait",wait_state)
	EnemyStateMachine:add_state("patrol",patrol_state)
end

function chase_do_first(args)
	--utils_log("Hummer: I'll get you")
end
function chase_do_end(args)
	--utils_log("Hummer: Where the fuck are you?")
end

function state1cond1()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	return (playerPos.x>20 or playerPos.x<-20 or playerPos.z>20 or playerPos.z<-20)
end

function state1fn(args,_ElapsedTime)
	
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	local playerPos = l_physXManager:get_character_controler_pos("player")
	
	args["point2go"] = playerPos
	
	go2point(args,_ElapsedTime)
	
end

function wait_do_first(args)
	--utils_log("Car: Uff! I escaped again!")
end
function wait_do_end(args)
	--utils_log("Car: Let's go again! JERONIMOOOOOOOOOOOOOOOOOOOOOOO!")
end
function state2cond1()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
	local playerPos = l_physXManager:get_character_controler_pos("player");
	return (playerPos.x<20 and playerPos.x>-20 and playerPos.z<20 and playerPos.z>-20)
	
end

function state2fn(args,elapsedTime)
end


function patrol_update(args,_ElapsedTime)
	
	
	args["point2go"] = waypoints[actual_waypoint]
	local _enemy_name = args["enemy_name"]
	go2point(args,_ElapsedTime)
	
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	local enemyPos = l_physXManager:get_character_controler_pos(_enemy_name);
	local dist = modulus(enemyPos-waypoints[actual_waypoint])
	
	if(dist<arrive_value) then
		actual_waypoint = get_next_waypoint(actual_waypoint)
	end
end

function patrol_first(args)
	--utils_log("patrol_state")
	
	actual_waypoint = 0

	mode = "circular"

	step = 1
end