dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateOff.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateTeleport.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateChase.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateAttack.lua")

class 'CFogEnemy' (CEnemy)
	function CFogEnemy:__init(_TreeNode)
		CEnemy.__init(self,_TreeNode)
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_WalkSpeed = _TreeNode:get_float_property("walk_speed", 1.0, false)
		self.m_RunSpeed = _TreeNode:get_float_property("run_speed", 5.0, false)
		self.m_AngularWalkSpeed = _TreeNode:get_float_property("angular_walk_speed", 1000.0, false)
		self.m_AngularRunSpeed = _TreeNode:get_float_property("angular_run_speed", 250.0, false)
		
		self.m_TimeToTeleport = _TreeNode:get_float_property("time_teleport", 1.0, false)
		self.m_TeleportDistance = _TreeNode:get_float_property("teleport_distance", 5.0, false)
		self.m_TimeNotLook = _TreeNode:get_float_property("time_not_look", 1.5, false)
		self.m_AngleToTeleport = _TreeNode:get_float_property("angle_teleport", 0.35, false)
		self.m_TeleportTimer = 0
		self.m_StopTimer = 0		
		self.m_TimerRotation = 0.0
		
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
		TeleportState:add_condition(FogTeleportToChaseCondition, "Chase")
		
		ChaseState = State.create(FogChaseUpdate)
		ChaseState:set_do_first_function(FogChaseFirst)
		ChaseState:set_do_end_function(FogChaseEnd)
		ChaseState:add_condition(FogChaseToTeleportCondition, "Teleport")
		ChaseState:add_condition(FogChaseToAttackCondition, "Attack")
		
		AttackState = State.create(FogAttackUpdate)
		AttackState:set_do_first_function(FogAttackFirst)
		AttackState:set_do_end_function(FogAttackEnd)
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Teleport", TeleportState)
		self.m_StateMachine:add_state("Chase", ChaseState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
--end