dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateOff.lua")
dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateIdle.lua")
dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateAttack.lua")

class 'CCagedAutomatonEnemy' (CEnemy)
	function CCagedAutomatonEnemy:__init(_TreeNode)
		CEnemy.__init(self,_TreeNode)
		self.m_Break = _TreeNode:get_bool_property("break", false, false)
		self.m_TimerToStop = 0.0
		self:SetCagedAutomatonStateMachine()
		self.m_StateMachine:start()
	end

	function CCagedAutomatonEnemy:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
	end

	function CCagedAutomatonEnemy:SetCagedAutomatonStateMachine()
		OffState = State.create(OffUpdateCagedAutomaton)
		OffState:set_do_first_function(OffFirstCagedAutomaton)
		OffState:set_do_end_function(OffEndCagedAutomaton)
		OffState:add_condition(OffToIdleConditionCagedAutomaton, "Idle")
		
		IdleState = State.create(IdleUpdateCagedAutomaton)
		IdleState:set_do_first_function(IdleFirstCagedAutomaton)
		IdleState:set_do_end_function(IdleEndCagedAutomaton)
		IdleState:add_condition(IdleToAttackConditionCagedAutomaton, "Attack")
		
		AttackState = State.create(AttackUpdateCagedAutomaton)
		AttackState:set_do_first_function(AttackFirstCagedAutomaton)
		AttackState:set_do_end_function(AttackEndCagedAutomaton)
		
		self.m_StateMachine:add_state("Off", OffState)
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Attack", AttackState)
	end
--end