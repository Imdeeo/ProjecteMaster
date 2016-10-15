dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateOff.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateIdle.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStatePatrol.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateChase.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateAlert.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateReturn.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateAttack.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateKill.lua")

class 'CAutomatonEnemy' (CEnemy)
	function CAutomatonEnemy:__init(_TreeNode,_LevelId)
		CEnemy.__init(self,_TreeNode,_LevelId)
		
		--HEAD_OBJECT_BONE_ID 31
		self.m_HeadBoneId = 31
		
		local l_Level = g_Engine:get_level_manager():get_level(_LevelId)
		self.m_PathFinding = l_Level:get_astar_manager():get_resource(_LevelId)
		self.m_TotalNodes = 0
		self.m_IndexPoint = 0
		self.m_IndexPathPatrolPoint = 0		
		self.m_Patrol = _TreeNode:get_bool_property("patrol", false)
		if self.m_Patrol then
			self.m_PatrolName = _TreeNode:get_psz_property("patrol_name", "")
			self.m_TotalPatrolNodes = self.m_PathFinding:get_total_patrol_nodes(self.m_PatrolName)
		end
		self.m_DetectedSound = false
		self.m_IsChasing = false
		self.m_IsReturn = false
		self.m_LastPositionPlayer = nil
		self.m_StandardAlertTime = _TreeNode:get_float_property("alert_time", 1.0) * math.pi
		self.m_LastPositionEnemy = nil
		self.m_IsCorrected = false
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_WalkSpeed = _TreeNode:get_float_property("walk_speed", 1.0)
		self.m_RunSpeed = _TreeNode:get_float_property("run_speed", 5.0)
		self.m_AngularWalkSpeed = _TreeNode:get_float_property("angular_walk_speed", 1000.0)
		self.m_AngularRunSpeed = _TreeNode:get_float_property("angular_run_speed", 250.0)
		
		self.m_DistanceToActivateRun = _TreeNode:get_float_property("distance_activate_run", 25.0)
		self.m_DistanceToActivateWalk = _TreeNode:get_float_property("distance_activate_walk", 15.0)
		self.m_DistanceToActivateCrouching = _TreeNode:get_float_property("distance_activate_crouching", 5.0)
		self.m_DistanceToChangeNodeWalking = _TreeNode:get_float_property("distance_change_node_walking", 2.0)
		self.m_DistanceToChangeNodeRunning = _TreeNode:get_float_property("distance_change_node_running", 2.0)
		
		self.m_TimerRotation = 0.0
		self.m_TimerRotation2 = 0.0
		self.m_alert_timer = 0
		
		self:SetAutomatonStateMachine()
		self.m_StateMachine:start()
	end

	function CAutomatonEnemy:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
	end

	function CAutomatonEnemy:SetAutomatonStateMachine()
		OffState = State.create(OffUpdateAutomaton)
		OffState:set_do_first_function(OffFirstAutomaton)
		OffState:set_do_end_function(OffEndAutomaton)
		OffState:add_condition(OffToIdleConditionAutomaton, "Idle")
		
		IdleState = State.create(IdleUpdateAutomaton)
		IdleState:set_do_first_function(IdleFirstAutomaton)
		IdleState:set_do_end_function(IdleEndAutomaton)
		IdleState:add_condition(IdleToPatrolConditionAutomaton, "Patrol")
		IdleState:add_condition(IdleToChaseConditionAutomaton, "Chase")
		IdleState:add_condition(ANYToAttackConditionAutomaton, "Attack")
		
		PatrolState = State.create(PatrolUpdateAutomaton)
		PatrolState:set_do_first_function(PatrolFirstAutomaton)
		PatrolState:set_do_end_function(PatrolEndAutomaton)
		PatrolState:add_condition(PatrolToChaseConditionAutomaton, "Chase")
		PatrolState:add_condition(PatrolToAlertConditionAutomaton, "Alert")
		PatrolState:add_condition(ANYToAttackConditionAutomaton, "Attack")
		
		ChaseState = State.create(ChaseUpdateAutomaton)
		ChaseState:set_do_first_function(ChaseFirstAutomaton)
		ChaseState:set_do_end_function(ChaseEndAutomaton)
		ChaseState:add_condition(ChaseToAlertConditionAutomaton, "Alert")
		ChaseState:add_condition(ChaseToAttackConditionAutomaton, "Attack")
		
		AlertState = State.create(AlertUpdateAutomaton)
		AlertState:set_do_first_function(AlertFirstAutomaton)
		AlertState:set_do_end_function(AlertEndAutomaton)
		AlertState:add_condition(AlertToChaseConditionAutomaton, "Chase")
		AlertState:add_condition(AlertToReturnConditionAutomaton, "Return")
		AlertState:add_condition(AlertToPatrolConditionAutomaton, "Patrol")
		AlertState:add_condition(ANYToAttackConditionAutomaton, "Attack")
		
		ReturnState = State.create(ReturnUpdateAutomaton)
		ReturnState:set_do_first_function(ReturnFirstAutomaton)
		ReturnState:set_do_end_function(ReturnEndAutomaton)
		ReturnState:add_condition(ReturnToIdleConditionAutomaton, "Idle")
		ReturnState:add_condition(ReturnToChaseConditionAutomaton, "Chase")
		ReturnState:add_condition(ANYToAttackConditionAutomaton, "Attack")
		
		AttackState = State.create(AttackUpdateAutomaton)
		AttackState:set_do_first_function(AttackFirstAutomaton)
		AttackState:set_do_end_function(AttackEndAutomaton)
		AttackState:add_condition(AttackToKillConditionAutomaton, "Kill")
		
		KillState = State.create(KillUpdateAutomaton)
		KillState:set_do_first_function(KillFirstAutomaton)
		KillState:set_do_end_function(KillEndAutomaton)
		KillState:add_condition(KillToOffConditionAutomaton, "Off")
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Patrol", PatrolState)
		self.m_StateMachine:add_state("Chase", ChaseState)
		self.m_StateMachine:add_state("Alert", AlertState)
		self.m_StateMachine:add_state("Return", ReturnState)
		self.m_StateMachine:add_state("Attack", AttackState)
		self.m_StateMachine:add_state("Kill", KillState)
	end
	
	function ANYToAttackConditionAutomaton(args)
		local l_Enemy = args["self"]
		return l_Enemy.m_State == "attack"
	end
	
	function CAutomatonEnemy:DetectPlayerNoise(_increment)
		local l_Distance = g_Player.m_RenderableObject:get_position():distance(self.m_RenderableObject:get_position())
		local l_Run = g_Player.m_InputManager:is_action_active("Run")
		local l_Crouch = g_Player.m_InputManager:is_action_active("Crouch")
		local l_MoveForward = g_Player.m_InputManager:is_action_active("MoveForward")
		local l_MoveBackward = g_Player.m_InputManager:is_action_active("MoveBackward")
		local l_MoveLeft = g_Player.m_InputManager:is_action_active("StrafeLeft")
		local l_MoveRight = g_Player.m_InputManager:is_action_active("StrafeRight")
		
		if (l_MoveForward or l_MoveBackward or l_MoveLeft or l_MoveRight) 
			and ((not l_Crouch and not l_Run and l_Distance <= self.m_DistanceToActivateWalk * _increment)
			or (l_Run and l_Distance <= self.m_DistanceToActivateRun * _increment)
			or (l_Crouch and l_Distance <= self.m_DistanceToActivateCrouching * _increment)) then
			self.m_DetectedSound = true
		else
			self.m_DetectedSound = false
		end
		
		return self.m_DetectedSound
	end
	
	function CAutomatonEnemy:GetActualPatrolPoint()		
		return self.m_PathFinding:get_patrol_point(self.m_PatrolName, self.m_IndexPathPatrolPoint)
	end
	
	function CAutomatonEnemy:IncrementePatrolPointIndex()
		if self.m_IndexPathPatrolPoint < self.m_TotalPatrolNodes - 1 then
			self.m_IndexPathPatrolPoint = self.m_IndexPathPatrolPoint + 1
		else
			self.m_IndexPathPatrolPoint = 0
		end
	end
	
	function CAutomatonEnemy:SearchForPath(_DesiredPos)
		self.m_IndexPoint = 0
		self.m_TotalNodes = self.m_PathFinding:search_for_path(self.m_RenderableObject:get_position(),_DesiredPos, self.m_Name)
	end
	
	function CAutomatonEnemy:IncrementPathPointIndex()
		if self.m_IndexPoint < self.m_TotalNodes - 1 then
			self.m_IndexPoint = self.m_IndexPoint + 1
			return true
		else
			return false
		end
	end
--end