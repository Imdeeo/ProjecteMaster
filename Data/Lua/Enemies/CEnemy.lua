dofile("Data\\Lua\\Utils\\state_machine.lua")

class 'CEnemy' (CLUAComponent)
	function CEnemy:__init(_TreeNode,_LevelId)
		local UABEngine = CUABEngine.get_instance()
		--utils_log("Enemy before get level")
		local l_Level = CUABEngine.get_instance():get_level_manager():get_level(_LevelId)
		--utils_log("Enemy after get level")
		self.m_Name = _TreeNode:get_psz_property("name", "")
		self.m_LayerName = _TreeNode:get_psz_property("layer", "")
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "")
		self.m_RenderableObject = l_Level:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)
		
		self.m_Timer = 0
		self.m_DefaultPosition = Vect3f(self.m_RenderableObject:get_position().x, self.m_RenderableObject:get_position().y, self.m_RenderableObject:get_position().z)
		----utils_log(self.m_Name.." POS X: "..self.m_DefaultPosition.x.." Y: "..self.m_DefaultPosition.y.." Z: "..self.m_DefaultPosition.z)
		self.m_DefaultRotation = self.m_RenderableObject:get_rotation()
		self.m_DefaultForward = self.m_DefaultRotation:get_forward_vector()
		self.m_State = "off"
		self.m_Awake = _TreeNode:get_bool_property("awake", false)
		self.m_ActualAnimation = 0
		self.m_LoseSanityValue = 0.0
		
		-- TODO: get group numbers somehow
		-- at the moment bit 0: plane, bit 1: objects, bit 2: triggers, bit 3: player
		self.m_PhysXGroups = 2 + 8 -- objects and player
		self.m_MaxDistance = _TreeNode:get_float_property("vision_distance", 5.0)
		self.m_MaxAngle = _TreeNode:get_float_property("vision_angle", 0.25) * math.pi
		self.m_HeadOffset = Vect3f(0.0, g_EnemyHeight + g_EnemyRadius * 2, 0.0)
		self.m_BlockingObjectName = nil
		self.m_DistanceToKill = _TreeNode:get_float_property("distance_kill", 1.0)
		
		-- distance, time, sanity amount
		self.m_LoseSanity = {}
		
		l_Param = _TreeNode:first_child()
		while l_Param ~= nil do
			table.insert(self.m_LoseSanity, Vect3f(l_Param:get_float_property("distance",0.0), l_Param:get_float_property("time",0.0), l_Param:get_float_property("sanity_amount",0.0)))
			l_Param = l_Param:get_next()
		end
		
		self.m_StateMachine = StateMachine.create()
		
		if self.m_Name ~= "Boss" then
			self.m_PhysXManager:create_character_controller(self.m_Name, g_EnemyHeight, g_EnemyRadius, 0.5, self.m_RenderableObject:get_position(),"FisicasAux", "Enemy")
		else
			self.m_PhysXManager:create_character_controller(self.m_Name, g_EnemyBossHeight, g_EnemyBossRadius, 0.5, self.m_RenderableObject:get_position(),"FisicasAux", "Boss")
		end
	end
	
	function CEnemy.Destroy(self)
		utils_log("Delete Enemy Name: "..self.m_Name)
		local l_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		l_PhysXManager:remove_actor(self.m_Name)
	end
	
	function CEnemy:Update(_ElapsedTime)
		--utils_log("CEnemy:Update")
	end
	
	function CEnemy:PlayerVisible(_Owner)
		local l_OwnerHeadPos = _Owner:get_position() + self.m_HeadOffset
		local l_PlayerPos = self.m_PhysXManager:get_character_controler_pos("player")
		--l_AuxPos = Vect3f(l_PlayerPos.x, l_PlayerPos.y - g_StandingOffset, l_PlayerPos.z)
		l_AuxPos = Vect3f(l_PlayerPos.x, l_PlayerPos.y, l_PlayerPos.z)
		
		-- not visible if too far
		local l_Dist = l_AuxPos:distance(l_OwnerHeadPos)
		if l_Dist > self.m_MaxDistance then
			return false
		end

		-- not visible if out of angle
		local l_PlayerDirection = l_AuxPos - l_OwnerHeadPos
		l_PlayerDirection:normalize(1.0)
		local l_Forward = _Owner:get_rotation():get_forward_vector()
		local l_Dot = l_Forward * l_PlayerDirection
		if l_Dot < math.cos(self.m_MaxAngle) then
			return false
		end

		-- not visible if behind an obstacle
		-- TODO: some raycasts from enemy's head to different parts of player
		local l_RaycastData = RaycastData()
		local l_Hit = self.m_PhysXManager:raycast(
			l_OwnerHeadPos, l_AuxPos,
			self.m_PhysXGroups, l_RaycastData
		)
		
		if l_Hit and l_RaycastData.actor_name ~= "player" then
			self.m_BlockingObjectName = l_RaycastData.actor_name
			return false
		else
			-- we do a new raycast to player foot to prove that it not has a little object between enemy and player
			l_AuxPos = Vect3f(l_PlayerPos.x, l_PlayerPos.y - g_StandingOffset, l_PlayerPos.z)
			l_Hit = self.m_PhysXManager:raycast(l_OwnerHeadPos, l_AuxPos, self.m_PhysXGroups, l_RaycastData)
			
			if l_Hit then
				self.m_BlockingObjectName = l_RaycastData.actor_name
				return false
			end
		end

		-- otherwise visible
		self.m_BlockingObjectName = nil
		return true
	end	
	
	function CEnemy:LoseSanity(_Distance)
		for i=1, table.maxn(self.m_LoseSanity)-1 do
			if _Distance <= self.m_LoseSanity[i].x and _Distance > self.m_LoseSanity[i+1].x and self.m_Timer >= self.m_LoseSanity[i].y then
				g_Player:ModifySanity(self.m_LoseSanity[i].z)
				self.m_Timer = 0
				break
			end
		end
	end
	
	function CEnemy:EnemyMove(_ElapsedTime)
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
	
	function CEnemy:EnemyWalk(_DesiredPos, _MoveSpeed, _PercentRotation, _ElapsedTime)
		-- enemy always walks in forward direction
		local l_Owner = self.m_RenderableObject;
		local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
		local l_EnemyPos = l_Owner:get_position()
		self.m_Velocity = Vect3f(l_EnemyForward.x * _MoveSpeed, self.m_Velocity.y, l_EnemyForward.z * _MoveSpeed)
		
		self:EnemyMove(_ElapsedTime)

		-- with the rotation, the enemy chases to the player
		local l_Direction = (_DesiredPos - l_EnemyPos):get_normalized(1)
		
		local angle_to_turn = self:CalculateAngleRotation(l_EnemyForward, l_Direction)
		self:EnemyRotation(angle_to_turn, _PercentRotation)
	end
	
	function CEnemy:EnemyRotation(_AngleToTurn, _PercentRotation)
		local l_Owner = self.m_RenderableObject;
		
		local quat_to_turn = Quatf()
		quat_to_turn:quat_from_yaw_pitch_roll(_AngleToTurn, 0.0, 0.0)

		local target_quat = l_Owner:get_rotation():slerpJU(l_Owner:get_rotation() * quat_to_turn, _PercentRotation)
		l_Owner:set_rotation(target_quat)
	end
	
	function CEnemy:RotateEnemyBone(_Bone, _DesiredPos, _PercentRotation)
		-- Seguimos al player con la mirada
		local l_Owner = self.m_RenderableObject;
		local l_EnemyPos = l_Owner:get_bone_position(_Bone) + l_Owner:get_position()
		local l_BoneRotation = l_Owner:get_bone_rotation(_Bone)
		local l_Direction = (_DesiredPos - l_EnemyPos)
		l_Direction.y = 0
		l_Direction:normalize(1)
		
		local angle_to_turn = self:CalculateAngleRotation(self.m_DefaultForward, l_Direction)
		
		if angle_to_turn ~= 0 then
			local quat_to_turn = Quatf()
			quat_to_turn:quat_from_yaw_pitch_roll(0.0, angle_to_turn, 0.0)		
			
			local target_quat = l_BoneRotation:slerpJU(quat_to_turn * l_BoneRotation, _PercentRotation)
			l_Owner:set_bone_rotation(target_quat, _Bone)
		end
	end
	
	function CEnemy:CalculateAngleRotation(_EnemyForward, _Direction)
		local l_Angle = _EnemyForward * _Direction
		if 1.0 - l_Angle < 0.01 then			
			return 0.0
		end
		
		local angle_to_turn = math.acos(l_Angle)
		local cross = _Direction ^ _EnemyForward
		if cross.y < 0.0 then
		  angle_to_turn = -angle_to_turn
		end	

		return angle_to_turn
	end	
	
	function CEnemy:ShowParticles(_ParticleName, _IsStart)
		local l_Owner = self.m_RenderableObject;
		local l_Level = g_Engine:get_level_manager():get_level(g_Player.m_ActualLevel)
		local l_Particle = l_Level:get_layer_manager():get_layer("particles"):get_resource(_ParticleName)
		
		if _IsStart then
			l_Particle:set_start(true)
			l_Particle:set_visible(true)
		end
		
		local l_EnemyPos = l_Owner:get_position()
		local l_HeadPos = l_Owner:get_bone_position(self.m_HeadBoneId) + Vect3f(0,-0.05,0.1)
		l_HeadPos = l_Owner:get_rotation():rotation_matrix() * l_HeadPos
		l_HeadPos.x = l_HeadPos.x * -1
		local l_Aux = (l_EnemyPos + l_HeadPos) / 2
		l_Particle:set_position(l_Aux)
	end
	
	function CEnemy:CheckPlayerDistance(_DesiredDistance)
		local l_PlayerPos = g_Player.m_RenderableObject:get_position()
		local l_Distance = l_PlayerPos:distance(self.m_RenderableObject:get_position())
		
		if l_Distance <= _DesiredDistance then
			return true
		else
			return false
		end
	end
--end