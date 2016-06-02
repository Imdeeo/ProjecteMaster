dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateOff.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateTeleport.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateChase.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateAttack.lua")

class 'CFogEnemy' (CEnemy)
	function CFogEnemy:__init(_TreeNode)
		utils_log("CFogEnemy_Init")
		CEnemy.__init(self,_TreeNode)
		
		
		self:SetFogAutomatonStateMachine()
		self.m_StateMachine:start()
	end

	function CFogEnemy:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
	end

	function CFogEnemy:SetFogAutomatonStateMachine()
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
		AttackState:add_condition(FogAttackToTeleportCondition, "Teleport")
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Teleport", TeleportState)
		self.m_StateMachine:add_state("Chase", ChaseState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
--end