dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateOff.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateTeleport.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateChase.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateAttack.lua")


FogAutomatonStateMachine = StateMachine.create()

function setFogAutomatonStateMachine()
	OffState = State.create(FogOffUpdate)
	OffState:set_do_first_function(FogOffFirst)
	OffState:set_do_end_function(FogOffEnd)
	OffState:add_condition(FogOffToTeleportCondition, "Teleport")
	
	TeleportState = State.create(FogTeleportUpdate)
	TeleportState:set_do_first_function(FogTeleportFirst)
	TeleportState:set_do_end_function(FogTeleportEnd)
	TeleportState:add_condition(FogTeleportToOffCondition, "Off")
	TeleportState:add_condition(FogTeleportToChaseCondition, "Chase")
	
	ChaseState = State.create(FogChaseUpdate)
	ChaseState:set_do_first_function(FogChaseFirst)
	ChaseState:set_do_end_function(FogChaseEnd)
	ChaseState:add_condition(FogChaseToTeleportCondition, "Teleport")
	ChaseState:add_condition(FogChaseToOffCondition, "Off")
	ChaseState:add_condition(FogChaseToAttackCondition, "Attack")
	
	AttackState = State.create(FogAttackUpdate)
	AttackState:set_do_first_function(FogAttackFirst)
	AttackState:set_do_end_function(FogAttackEnd)
	AttackState:add_condition(FogAttackToChaseCondition, "Chase")
	AttackState:add_condition(FogAttackToOffCondition, "Off")
	
	
	FogAutomatonStateMachine:add_state("Off", OffState)
	FogAutomatonStateMachine:add_state("Teleport", TeleportState)
	FogAutomatonStateMachine:add_state("Chase", ChaseState)
	FogAutomatonStateMachine:add_state("Attack", AttackState)
end