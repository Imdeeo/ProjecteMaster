dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStateOff.lua")
dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStateIdle.lua")
dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStateLooking.lua")
dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStatePatrol.lua")
dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStateAttack.lua")
dofile("Data\\Lua\\Enemies\\BossEnemy\\BossStateKill.lua")

class 'CBossEnemy' (CEnemy)
	function CBossEnemy:__init(_TreeNode,_LevelId)
		CEnemy.__init(self,_TreeNode,_LevelId)
		
		local l_Level = g_Engine:get_level_manager():get_level(_LevelId)
		self.m_PathFinding = l_Level:get_astar_manager():get_resource(_LevelId)
		self.m_IndexPathPatrolPoint = 0		
		self.m_Patrol = _TreeNode:get_bool_property("patrol", false)
		if self.m_Patrol then
			self.m_PatrolName = _TreeNode:get_psz_property("patrol_name", "")
			self.m_TotalPatrolNodes = self.m_PathFinding:get_total_patrol_nodes(self.m_PatrolName)
		end
		self.m_IsCorrected = false
		self.m_Looking = false
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_WalkSpeed = _TreeNode:get_float_property("walk_speed", 1.0)
		self.m_AngularWalkSpeed = _TreeNode:get_float_property("angular_walk_speed", 1000.0)
		self.m_DistanceToChangeNodeWalking = _TreeNode:get_float_property("distance_change_node_walking", 2.0)
		
		self.m_TimerRotation = 0.0
		
		self:SetBossStateMachine()
		self.m_StateMachine:start()
	end

	function CBossEnemy.Destroy(self)
		CEnemy.Destroy(self)
	end
	
	function CBossEnemy:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
	end

	function CBossEnemy:SetBossStateMachine()
		OffState = State.create(OffUpdateBoss)
		OffState:set_do_first_function(OffFirstBoss)
		OffState:set_do_end_function(OffEndBoss)
		OffState:add_condition(OffToIdleConditionBoss, "Idle")
		
		IdleState = State.create(IdleUpdateBoss)
		IdleState:set_do_first_function(IdleFirstBoss)
		IdleState:set_do_end_function(IdleEndBoss)
		IdleState:add_condition(IdleToLookingConditionBoss, "Looking")
		IdleState:add_condition(IdleToPatrolConditionBoss, "Patrol")
		
		LookingState = State.create(LookingUpdateBoss)
		LookingState:set_do_first_function(LookingFirstBoss)
		LookingState:set_do_end_function(LookingEndBoss)
		LookingState:add_condition(LookingToIdleConditionBoss, "Idle")
		
		PatrolState = State.create(PatrolUpdateBoss)
		PatrolState:set_do_first_function(PatrolFirstBoss)
		PatrolState:set_do_end_function(PatrolEndBoss)
		PatrolState:add_condition(PatrolToAttackConditionBoss, "Attack")
		PatrolState:add_condition(PatrolToOffConditionBoss, "Off")
		
		AttackState = State.create(AttackUpdateBoss)
		AttackState:set_do_first_function(AttackFirstBoss)
		AttackState:set_do_end_function(AttackEndBoss)
		AttackState:add_condition(AttackToKillConditionBoss, "Kill")
		
		KillState = State.create(KillUpdateBoss)
		KillState:set_do_first_function(KillFirstBoss)
		KillState:set_do_end_function(KillEndBoss)
		KillState:add_condition(KillToOffConditionBoss, "Off")
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Looking", LookingState)
		self.m_StateMachine:add_state("Patrol", PatrolState)
		self.m_StateMachine:add_state("Attack", AttackState)
		self.m_StateMachine:add_state("Kill", KillState)
	end
	
	function CBossEnemy:GetActualPatrolPoint()		
		return self.m_PathFinding:get_patrol_point(self.m_PatrolName, self.m_IndexPathPatrolPoint)
	end
	
	function CBossEnemy:IncrementePatrolPointIndex()
		if self.m_IndexPathPatrolPoint < self.m_TotalPatrolNodes - 1 then
			self.m_IndexPathPatrolPoint = self.m_IndexPathPatrolPoint + 1
		else
			self.m_IndexPathPatrolPoint = 0
		end
	end
--end