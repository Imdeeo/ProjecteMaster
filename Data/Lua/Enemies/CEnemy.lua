dofile("Data\\Lua\\Utils\\GVars.lua") --// Global Variables
dofile("Data\\Lua\\Utils\\state_machine.lua")

class 'CEnemy' (CLUAComponent)
	function CEnemy:__init(_TreeNode)
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)
		
		self.m_Timer = 0
		self.m_DefaultPosition = Vect3f(self.m_RenderableObject:get_position().x, self.m_RenderableObject:get_position().y, self.m_RenderableObject:get_position().z)
		self.m_DefaultForward = self.m_RenderableObject:get_rotation():get_forward_vector()
		self.m_State = "off"
		self.m_Awake = _TreeNode:get_bool_property("awake", false, false)
		self.m_ActualAnimation = 0
		
		-- TODO: get group numbers somehow
		-- at the moment bit 0: plane, bit 1: objects, bit 2: triggers, bit 3: player
		self.m_PhysXGroups = 2 + 8 -- objects and player
		self.m_MaxDistance = _TreeNode:get_float_property("vision_distance", 5.0, false)
		self.m_MaxAngle = _TreeNode:get_float_property("vision_angle", 0.25, false) * math.pi
		self.m_HeadOffset = Vect3f(0.0, g_EnemyHeight + g_EnemyRadius * 2, 0.0)
		self.m_BlockingObjectName = nil
		self.m_DistanceToKill = _TreeNode:get_float_property("distance_kill", 1.0, false)
		
		-- distance, time, sanity amount
		self.m_LoseSanity = {}
		for i = 0, _TreeNode:get_num_children()-1 do
			local l_Param = _TreeNode:get_child(i)
			table.insert(self.m_LoseSanity, Vect3f(l_Param:get_float_property("distance",0.0,false), l_Param:get_float_property("time",0.0,false), l_Param:get_float_property("sanity_amount",0.0,false)))
		end
		
		self.m_StateMachine = StateMachine.create()
		if (not UABEngine:get_lua_reloaded()) then
			self.m_PhysXManager:create_character_controller(self.m_Name, g_EnemyHeight, g_EnemyRadius, 0.5, self.m_RenderableObject:get_position(),"controllerMaterial", "Enemy")
		end
	end
	
	function CEnemy:Update(_ElapsedTime)
		utils_log("CEnemy:Update")
	end
	
	function CEnemy:PlayerVisible(_Owner)
		local l_OwnerHeadPos = _Owner:get_position() + self.m_HeadOffset
		local l_PlayerPos = self.m_PhysXManager:get_character_controler_pos("player")
		
		-- not visible if too far
		local l_Dist = l_PlayerPos:distance(l_OwnerHeadPos)
		if l_Dist > self.m_MaxDistance then
			return false
		end

		-- not visible if out of angle
		local l_PlayerDirection = l_PlayerPos - l_OwnerHeadPos
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
	
	function CEnemy:RotateEnemyHead(_Owner, _DesiredPos)
		-- Seguimos al player con la mirada
		local l_EnemyForward = _Owner:get_rotation():get_forward_vector():get_normalized(1)
		local l_EnemyPos = _Owner:get_position()
		
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
		quat_to_turn:quat_from_yaw_pitch_roll(0.0, angle_to_turn, 0.0)		
		_Owner:set_head_bone_rotation(quat_to_turn)
	end
	
	function CEnemy:LoseSanity(_Distance)
		for i=1, table.maxn(self.m_LoseSanity) do
			if _Distance <= self.m_LoseSanity[i].x and _Distance > self.m_LoseSanity[i+1].x and self.m_Timer >= self.m_LoseSanity[i].y then
				g_Player:ModifySanity(self.m_LoseSanity[i].z)
				self.m_Timer = 0
				break
			end
		end
	end
--end