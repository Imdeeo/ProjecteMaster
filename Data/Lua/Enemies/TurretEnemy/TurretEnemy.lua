dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateOff.lua")
dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateIdle.lua")
dofile("Data\\Lua\\Enemies\\TurretEnemy\\TurretStateAttack.lua")

class 'CTurretEnemy' (CEnemy)
	function CTurretEnemy:__init(_TreeNode)
		CEnemy.__init(self,_TreeNode)
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
	
	function CTurretEnemy:EnemyRotation(_DesiredPos, _MoveSpeed, _PercentRotation, _ElapsedTime)
		-- enemy always walks in forward direction
		local l_Owner = self.m_RenderableObject;
		local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
		local l_EnemyPos = l_Owner:get_position()
		
		-- with the rotation, the enemy chases to the player
		local l_Direction = (_DesiredPos - l_EnemyPos):get_normalized(1)	
		local l_Angle = l_EnemyForward * l_Direction
		if 1.0 - l_Angle < 0.01 then
		  return
		end
		
		local angle_to_turn = math.acos(l_Angle)
		local cross = l_Direction ^ l_EnemyForward
		if cross.y < 0.0 then
		  angle_to_turn = -angle_to_turn
		end
		
		local quat_to_turn = Quatf()
		quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)		
		
		local target_quat = l_Owner:get_rotation():slerp(l_Owner:get_rotation() * quat_to_turn, _PercentRotation)
		l_Owner:set_rotation(target_quat)
	end
--end