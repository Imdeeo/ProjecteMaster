dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateOff.lua")
dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateIdle.lua")
dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateAttack.lua")

class 'CTurretEnemy' (CEnemy)
	function CTurretEnemy:__init(_TreeNode,_LevelId)
		CEnemy.__init(self,_TreeNode,_LevelId)
		
		--HEAD_OBJECT_BONE_ID 31
		self.m_HeadBoneId = 23
		--BACK_OBJECT_BONE_ID 0
		self.m_BackBoneId = 0
		self.m_AngularSpeed = _TreeNode:get_float_property("angular_speed", 1000.0)
		self.m_DistanceToAttack = _TreeNode:get_float_property("distance_attack", 2.0)
		self.m_DistanceToKill = _TreeNode:get_float_property("distance_kill", 1.0)
		self.m_TimerRotation = 0.0
		self.m_TimerToStop = 0.0
		
		self.m_DefaultRotation = nil
		
		self:SetTurretStateMachine()
		self.m_StateMachine:start()
	end

	function CTurretEnemy:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
	end

	function CTurretEnemy:SetTurretStateMachine()
		OffState = State.create(OffUpdateTurret)
		OffState:set_do_first_function(OffFirstTurret)
		OffState:set_do_end_function(OffEndTurret)
		OffState:add_condition(OffToIdleConditionTurret, "Idle")
		
		IdleState = State.create(IdleUpdateTurret)
		IdleState:set_do_first_function(IdleFirstTurret)
		IdleState:set_do_end_function(IdleEndTurret)
		IdleState:add_condition(IdleToAttackConditionTurret, "Attack")
				
		AttackState = State.create(AttackUpdateTurret)
		AttackState:set_do_first_function(AttackFirstTurret)
		AttackState:set_do_end_function(AttackEndTurret)
		AttackState:add_condition(AttackToIdleConditionTurret, "Idle")
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
--end