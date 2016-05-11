dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Player\\PlayerStateIdle.lua")
dofile("Data\\Lua\\Player\\PlayerStateMoving.lua")
dofile("Data\\Lua\\Player\\PlayerStateJumping.lua")
dofile("Data\\Lua\\Player\\PlayerStateInteracting.lua")
dofile("Data\\Lua\\Player\\PlayerStateDead.lua")

PlayerStateMachine = StateMachine.create()

function setPlayerStateMachine()
	utils_log("Set PlayerStateMachine")
	
	IdleState = State.create(IdleUpdate)
	IdleState:set_do_first_function(IdleFirst)
	IdleState:set_do_end_function(IdleEnd)
	IdleState:add_condition(IdleToMovingCondition, "Moving")
	IdleState:add_condition(IdleToJumpingCondition, "Jumping")
	
	MovingState = State.create(MovingUpdate)
	MovingState:set_do_first_function(MovingFirst)
	MovingState:set_do_end_function(MovingEnd)
	MovingState:add_condition(MovingToIdleCondition, "Idle")
	MovingState:add_condition(MovingToJumpingCondition, "Jumping")
	
	JumpingState = State.create(JumpingUpdate)
	JumpingState:set_do_first_function(JumpingFirst)
	JumpingState:set_do_end_function(JumpingEnd)
	JumpingState:add_condition(JumpingToIdleCondition, "Idle")
	
	InteractingState = State.create(InteractingUpdate)
	InteractingState:set_do_first_function(InteractingFirst)
	InteractingState:set_do_end_function(InteractingEnd)
	
	DeadState = State.create(DeadUpdate)
	DeadState:set_do_first_function(DeadFirst)
	DeadState:set_do_end_function(DeadEnd)
	
	PlayerStateMachine:add_state("Idle", IdleState)
	PlayerStateMachine:add_state("Moving", MovingState)
	PlayerStateMachine:add_state("Jumping", JumpingState)
	PlayerStateMachine:add_state("Interacting", InteractingState)
	PlayerStateMachine:add_state("Dead", DeadState)
end