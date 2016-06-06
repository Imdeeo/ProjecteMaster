dofile("Data\\Lua\\Utils\\state_machine.lua")
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
	
	function CAutomatonEnemy:PlayerVisible(m_Owner)
		local l_OwnerHeadPos = m_Owner:get_position() + self.m_HeadOffset
		local l_PlayerPos = self.m_PhysXManager:get_character_controler_pos("player")
		
		-- not visible if too far
		local l_Dist = l_PlayerPos:distance(l_OwnerHeadPos)
		if l_Dist > self.m_MaxDistance then
			return false
		end

		-- not visible if out of angle
		local l_PlayerDirection = l_PlayerPos - l_OwnerHeadPos
		l_PlayerDirection:normalize(1.0)
		local l_Forward = m_Owner:get_rotation():get_forward_vector()
		local l_Dot = l_Forward * l_PlayerDirection
		if l_Dot < math.cos(self.m_MaxAngle) then
		  return false
		end

		-- not visible if behind an obstacle
		-- TODO: some raycasts from enemy's head to different parts of player
		local l_RaycastData = RaycastData()
		local l_Hit = self.m_PhysXManager:raycast(
			l_OwnerHeadPos, l_PlayerPos,
			self.m_PhysXGroups, l_RaycastData
		)
		
		if l_Hit and l_RaycastData.actor_name ~= "player" then
		  self.m_BlockingObjectName = l_RaycastData.actor_name
		  return false
		end

		-- otherwise visible
		self.m_BlockingObjectName = nil
		return true
	end
--end