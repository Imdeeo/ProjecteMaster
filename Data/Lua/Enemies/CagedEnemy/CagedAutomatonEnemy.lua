dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateOff.lua")
dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateIdle.lua")
dofile("Data\\Lua\\Enemies\\CagedEnemy\\CagedAutomatonStateAttack.lua")

class 'CCagedAutomatonEnemy' (CEnemy)
	function CCagedAutomatonEnemy:__init(_TreeNode,_LevelId)
		CEnemy.__init(self,_TreeNode,_LevelId)
		self.m_Break = _TreeNode:get_bool_property("break", false)
		self.m_AngularSpeed = _TreeNode:get_float_property("angular_speed", 1000.0)
		self.m_TimerRotation = 0.0
		self.m_TimerToStop = 0.0
		--HEAD_OBJECT_BONE_ID 31
		self.m_HeadBoneId = 31

		self.m_BeatSoundEvent = SoundEvent()
		self.m_BeatSoundEvent.event_name = "Automaton_Start_Beating"
		self.m_StopBeatSoundEvent = SoundEvent()
		self.m_StopBeatSoundEvent.event_name = "Automaton_Stop_Beating"
		
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