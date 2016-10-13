dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateOff.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateChase.lua")
dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateAttack.lua")

class 'CFogEnemy' (CEnemy)
	function CFogEnemy:__init(_TreeNode,_LevelId)
		CEnemy.__init(self,_TreeNode,_LevelId)
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_RunSpeed = _TreeNode:get_float_property("run_speed", 5.0)
		self.m_AngularRunSpeed = _TreeNode:get_float_property("angular_run_speed", 250.0)
		
		self.m_TimeToTeleport = _TreeNode:get_float_property("time_teleport", 1.0)
		self.m_TeleportDistance = _TreeNode:get_float_property("teleport_distance", 5.0)
		self.m_TimeNotLook = _TreeNode:get_float_property("time_not_look", 1.5)
		self.m_AngleToTeleport = _TreeNode:get_float_property("angle_teleport", 0.35)
		self.m_TeleportTimer = 0
		self.m_StopTimer = 0		
		self.m_TimerRotation = 0.0		
		
		self:SetFogAutomatonStateMachine()
		self.m_StateMachine:start()
		self.m_Off = true
		self.m_Attack = false
		self.m_OffPos = Vect3f(13.5, 0.1, 13.9)
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
		OffState:add_condition(FogOffToChaseCondition, "Chase")
		
		ChaseState = State.create(FogChaseUpdate)
		ChaseState:set_do_first_function(FogChaseFirst)
		ChaseState:set_do_end_function(FogChaseEnd)
		ChaseState:add_condition(FogChaseToOffCondition, "Off")
		ChaseState:add_condition(FogChaseToAttackCondition, "Attack")
		
		AttackState = State.create(FogAttackUpdate)
		AttackState:set_do_first_function(FogAttackFirst)
		AttackState:set_do_end_function(FogAttackEnd)
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Chase", ChaseState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
--end