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
		self.m_PathFindig:load_map("Data\\level_"..g_Engine:get_level_loaded().."\\pathfinding.xml")
		self.m_PatrolName = _TreeNode:get_psz_property("patrol_name", "", false)
		self.m_LastPositionPlayer = nil
		self.m_TotalNodes = 0
		self.m_IsChasing = false
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
--end