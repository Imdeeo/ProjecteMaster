dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateOff.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateIdle.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStatePatrol.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateChase.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateAlert.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateReturn.lua")
dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateAttack.lua")

class 'CAutomatonEnemy' (CEnemy)
	function CAutomatonEnemy:__init(_TreeNode)
		CEnemy.__init(self,_TreeNode)
		
		self.m_PathFindig = CUABEngine.get_instance():get_astar_manager()
		self.m_TotalNodes = 0
		self.m_Patrol = _TreeNode:get_bool_property("patrol", false, false)
		self.m_PatrolName = _TreeNode:get_psz_property("patrol_name", "", false)
		self.m_IsChasing = false
		self.m_IsReturn = false
		self.m_LastPositionPlayer = nil
		self.m_StandardAlertTime = _TreeNode:get_float_property("alert_time", 1.0, false) * math.pi
		self.m_LastPositionEnemy = nil
		
		self.m_Velocity = Vect3f(0,0,0)
		self.m_Gravity = -9.81
		self.m_WalkSpeed = _TreeNode:get_float_property("walk_speed", 1.0, false)
		self.m_RunSpeed = _TreeNode:get_float_property("run_speed", 5.0, false)
		self.m_AngularWalkSpeed = _TreeNode:get_float_property("angular_walk_speed", 1000.0, false)
		self.m_AngularRunSpeed = _TreeNode:get_float_property("angular_run_speed", 250.0, false)
		
		self.m_DistanceToActivateRun = _TreeNode:get_float_property("distance_activate_run", 25.0, false)
		self.m_DistanceToActivateWalk = _TreeNode:get_float_property("distance_activate_walk", 15.0, false)
		self.m_DistanceToActivateCrouching = _TreeNode:get_float_property("distance_activate_crouching", 5.0, false)
		self.m_DistanceToChangeNodeWalking = _TreeNode:get_float_property("distance_change_node_walking", 2.0, false)
		self.m_DistanceToChangeNodeRunning = _TreeNode:get_float_property("distance_change_node_running", 2.0, false)
		
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
		
		PatrolState = State.create(PatrolUpdateAutomaton)
		PatrolState:set_do_first_function(PatrolFirstAutomaton)
		PatrolState:set_do_end_function(PatrolEndAutomaton)
		PatrolState:add_condition(PatrolToChaseConditionAutomaton, "Chase")
		PatrolState:add_condition(PatrolToAlertConditionAutomaton, "Alert")
		
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
		
		ReturnState = State.create(ReturnUpdateAutomaton)
		ReturnState:set_do_first_function(ReturnFirstAutomaton)
		ReturnState:set_do_end_function(ReturnEndAutomaton)
		ReturnState:add_condition(ReturnToIdleConditionAutomaton, "Idle")
		ReturnState:add_condition(ReturnToChaseConditionAutomaton, "Chase")
		
		AttackState = State.create(AttackUpdateAutomaton)
		AttackState:set_do_first_function(AttackFirstAutomaton)
		AttackState:set_do_end_function(AttackEndAutomaton)
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Patrol", PatrolState)
		self.m_StateMachine:add_state("Chase", ChaseState)
		self.m_StateMachine:add_state("Alert", AlertState)
		self.m_StateMachine:add_state("Return", ReturnState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
	
	function CAutomatonEnemy:EnemyMove(_ElapsedTime)
		local l_Owner = self.m_RenderableObject;
		
		-- Calculate the enemy speed
		local l_PlayerDisplacement = Vect3f(self.m_Velocity.x, self.m_Velocity.y + self.m_Gravity * _ElapsedTime, self.m_Velocity.z)
		
		--// Move the character controller
		local l_PreviousControllerPosition = self.m_PhysXManager:get_character_controler_pos(self.m_Name)
		l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
		self.m_PhysXManager:character_controller_move(self.m_Name, l_PlayerDisplacement, _ElapsedTime)
		
		--// Assign to the character the controller's position
		local l_NewControllerPosition = self.m_PhysXManager:get_character_controler_pos(self.m_Name)
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
		l_Owner:set_position(l_NewControllerPosition)
		
		--// Save speed in last update so we can create acceleration
		local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
		self.m_Velocity = l_Displacement/_ElapsedTime
	end
	
	function CAutomatonEnemy:EnemyWalk(_DesiredPos, _MoveSpeed, _PercentRotation, _ElapsedTime)
		-- enemy always walks in forward direction
		local l_Owner = self.m_RenderableObject;
		local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
		local l_EnemyPos = l_Owner:get_position()
		self.m_Velocity = Vect3f(l_EnemyForward.x * _MoveSpeed, self.m_Velocity.y, l_EnemyForward.z * _MoveSpeed)
		
		self:EnemyMove(_ElapsedTime)

		-- with the rotation, the enemy chases to the player
		local l_Direction = (_DesiredPos - l_EnemyPos):get_normalized(1)	
		
		local angle_to_turn = self:CalculateAngleRotation(l_EnemyForward, l_Direction)
		if angle_to_turn ~= nil then
			self:EnemyRotation(angle_to_turn, _PercentRotation)
		end
	end
	
	function CAutomatonEnemy:EnemyRotation(_AngleToTurn, _PercentRotation)
		local l_Owner = self.m_RenderableObject;
		
		local quat_to_turn = Quatf()
		quat_to_turn:quat_from_yaw_pitch_roll(_AngleToTurn, 0.0, 0.0)

		local target_quat = l_Owner:get_rotation():slerp(l_Owner:get_rotation() * quat_to_turn, _PercentRotation)
		l_Owner:set_rotation(target_quat)
	end
	
	function CAutomatonEnemy:CalculateAngleRotation(_EnemyForward, _Direction)
		local l_Angle = _EnemyForward * _Direction
		if 1.0 - l_Angle < 0.01 then
			return nil
		end
		
		local angle_to_turn = math.acos(l_Angle)
		local cross = _Direction ^ _EnemyForward
		if cross.y < 0.0 then
		  angle_to_turn = -angle_to_turn
		end	

		return angle_to_turn		
	end
	
	function CAutomatonEnemy:DetectPlayerNoise(_increment)
		local l_Distance = g_Player.m_RenderableObject:get_position():distance(self.m_RenderableObject:get_position())
		local l_Run = g_Player.m_InputManager:is_action_active("Run")
		local l_Crouch = g_Player.m_InputManager:is_action_active("Crouch")
		
		if (l_Run and l_Distance <= self.m_DistanceToActivateRun * _increment)
			or (not l_Crouch and not l_Run and l_Distance <= self.m_DistanceToActivateWalk * _increment)
			or (l_Crouch and l_Distance <= self.m_DistanceToActivateCrouching * _increment) then
			return true
		else
			return false
		end
	end
--end