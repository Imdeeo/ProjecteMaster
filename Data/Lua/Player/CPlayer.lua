--// Global Variables
dofile("Data\\Lua\\Utils\\GVars.lua")
--// RaycastData
dofile("Data\\Lua\\Raycast.lua")
--// StateMachine
dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Player\\PlayerStateIdle.lua")
dofile("Data\\Lua\\Player\\PlayerStateMoving.lua")
dofile("Data\\Lua\\Player\\PlayerStateCorrecting.lua")
dofile("Data\\Lua\\Player\\PlayerStateCrouching.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbingStart.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbingIdle.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbingUp.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbingDown.lua")
dofile("Data\\Lua\\Player\\PlayerStateJumping.lua")
dofile("Data\\Lua\\Player\\PlayerStateFalling.lua")
dofile("Data\\Lua\\Player\\PlayerStateInteracting.lua")
dofile("Data\\Lua\\Player\\PlayerStateSinging.lua")
dofile("Data\\Lua\\Player\\PlayerStateDead.lua")
dofile("Data\\Lua\\Player\\PlayerStatePuzzle.lua")

--Bone #0: BrazosJaheem
--Bone #1: CATRigHub001
--Bone #2: CATRigLArm1
--Bone #3: CATRigLArm2
--Bone #4: CATRigLArmPalm
--Bone #5: CATRigLArmDigit11
--Bone #6: CATRigLArmDigit12
--Bone #7: CATRigLArmDigit13
--Bone #8: CATRigLArmDigit21
--Bone #9: CATRigLArmDigit22
--Bone #10: CATRigLArmDigit23
--Bone #11: CATRigLArmDigit31
--Bone #12: CATRigLArmDigit32
--Bone #13: CATRigLArmDigit33
--Bone #14: CATRigLArmDigit41
--Bone #15: CATRigLArmDigit42
--Bone #16: CATRigLArmDigit43
--Bone #17: CATRigLArmDigit51
--Bone #18: CATRigLArmDigit52
--Bone #19: CATRigLArmDigit53
--Bone #20: CATRigObjetoIzq
--Bone #21: CATRigRArm1
--Bone #22: CATRigRArm2
--Bone #23: CATRigRArmPalm
--Bone #24: CATRigRArmDigit11
--Bone #25: CATRigRArmDigit12
--Bone #26: CATRigRArmDigit13
--Bone #27: CATRigRArmDigit21
--Bone #28: CATRigRArmDigit22
--Bone #29: CATRigRArmDigit23
--Bone #30: CATRigRArmDigit31
--Bone #31: CATRigRArmDigit32
--Bone #32: CATRigRArmDigit33
--Bone #33: CATRigRArmDigit41
--Bone #34: CATRigRArmDigit42
--Bone #35: CATRigRArmDigit43
--Bone #36: CATRigRArmDigit51
--Bone #37: CATRigRArmDigit52
--Bone #38: CATRigRArmDigit53
--Bone #39: CATRigObjetoDcha
--Bone #40: CATRig1Bone001
--Bone #41: CATRig1Bone002

class 'CPlayer' (CLUAComponent)
	function CPlayer:__init(_TreeNode)
		self.m_ActualLevel = "Player"
		self.m_AlreadyInitialized = false
		local UABEngine = CUABEngine.get_instance()
		local l_Level = g_Engine:get_level_manager():get_level("Player")
		self.m_Name = _TreeNode:get_psz_property("name", "")
		self.m_LayerName = _TreeNode:get_psz_property("layer", "")
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "")
		self.m_RenderableObject = l_Level:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		CLUAComponent.__init(self, self.m_Name, self.m_RenderableObject)
	
		self.m_CameraControllerName= _TreeNode:get_psz_property("camera_controller", "")
		self.m_CameraController = UABEngine:get_camera_controller_manager():get_resource(self.m_CameraControllerName)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "")
		
		-- distance, time, sanity amount
		self.m_SanityEffects = {}
		l_Param = _TreeNode:first_child()
		while l_Param ~= nil do
			if l_Param:get_name() == "vortex" then
				l_VortexEffects = {}
				table.insert(l_VortexEffects, "vortex")
				table.insert(l_VortexEffects, l_Param:get_psz_property("type",""))
				table.insert(l_VortexEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_VortexEffects, l_Param:get_float_property("end",0.0))
				table.insert(l_VortexEffects, l_Param:get_psz_property("material",""))
				table.insert(l_VortexEffects, l_Param:get_float_property("variation",0.0))
				table.insert(l_VortexEffects, l_Param:get_float_property("default_value",0.0))
				table.insert(self.m_SanityEffects, l_VortexEffects)
			elseif l_Param:get_name() == "stain" then
				l_StainEffects = {}
				table.insert(l_StainEffects, "stain")
				table.insert(l_StainEffects, l_Param:get_psz_property("type",""))
				table.insert(l_StainEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_StainEffects, l_Param:get_float_property("end",0.0))
				table.insert(l_StainEffects, l_Param:get_psz_property("renderable_object",""))
				table.insert(self.m_SanityEffects, l_StainEffects)
			elseif l_Param:get_name() == "vignetting" then
				l_VignettinEffects = {}
				table.insert(l_VignettinEffects, "vignetting")
				table.insert(l_VignettinEffects, l_Param:get_psz_property("type",""))
				table.insert(l_VignettinEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_VignettinEffects, l_Param:get_float_property("end",0.0))
				table.insert(l_VignettinEffects, l_Param:get_psz_property("material",""))
				table.insert(l_VignettinEffects, l_Param:get_float_property("default_value",0.0))
				table.insert(self.m_SanityEffects, l_VignettinEffects)
			elseif l_Param:get_name() == "fov" then
				l_FovEffects = {}
				table.insert(l_FovEffects, "fov")
				table.insert(l_FovEffects, l_Param:get_psz_property("type",""))
				table.insert(l_FovEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_FovEffects, l_Param:get_float_property("end",0.0))
				table.insert(l_FovEffects, l_Param:get_float_property("start_value",0.0))
				table.insert(l_FovEffects, l_Param:get_float_property("end_value",0.0))
				table.insert(l_FovEffects, l_Param:get_float_property("default_value",0.0))
				table.insert(self.m_SanityEffects, l_FovEffects)
			elseif l_Param:get_name() == "velocity" then
				l_VelocityEffects = {}
				table.insert(l_VelocityEffects, "velocity")
				table.insert(l_VelocityEffects, l_Param:get_psz_property("type",""))				
				table.insert(l_VelocityEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_VelocityEffects, l_Param:get_float_property("end",0.0))
				table.insert(self.m_SanityEffects, l_VelocityEffects)
			--[[elseif l_Param:get_name() == "control" then
				l_ControlEffects = {}
				table.insert(l_ControlEffects, "control")
				table.insert(l_ControlEffects, l_Param:get_psz_property("type",""))
				table.insert(l_ControlEffects, l_Param:get_float_property("start",0.0))
				table.insert(l_ControlEffects, l_Param:get_float_property("end",0.0))
				table.insert(self.m_SanityEffects, l_ControlEffects)]]
			end
			l_Param = l_Param:get_next()
		end
		
		self.m_SoundManager = UABEngine:get_sound_manager()
		-- unregister old speaker before assigning new renderable object
		--self.m_SoundManager:unregister_speaker(self.m_RenderableObject)
		
		self.m_CinematicManager = l_Level:get_cinematic_manager()
		self.m_InputManager = UABEngine:get_input_manager()
		self.m_PhysXManager = UABEngine:get_physX_manager()
		self.m_SoundManager:register_speaker(self.m_RenderableObject)
		
		self.m_JumpSoundEvent = SoundEvent()
		self.m_JumpSoundEvent.event_name = "Jump"
		self.m_Velocity = Vect3f(0.0, 0.0, 0.0)
		self.m_Gravity = g_Gravity		
		self.m_DefaultSpeed = 3.0
		self.m_Speed = self.m_DefaultSpeed
		self.m_Sanity = 100.0
		self.m_MaxSanity = 100.0
		self.m_TimerVortex = 0
		
		self.m_IsSinging = false
		self.m_IsWindedUp = false
		self.m_IsCorrecting = false
		self.m_IsClimbing = false
		self.m_ClimbingUp = false
		self.m_ClimbingDown = false
		self.m_IsInteracting = false
		self.m_IsPuzzle = false
		self.m_IsDead = false
		
		self.m_Target = nil
		self.m_TargetPosOffset = Vect3f(1.0, 0.0, 0.0)
		self.m_TargetPosition = Vect3f(1.0, 0.0, 0.0)
		self.m_ForwardCamera = Vect3f(1.0,0.0,0.0)
		self.m_UpCamera = Vect3f(1.0,0.0,0.0)
		self.m_ItemName = ""--"Artilufacto"
		self.m_Item = nil--CUABEngine.get_instance():get_layer_manager():get_resource("solid"):get_resource(self.m_ItemName)
		self.m_TargetYaw = 0.0
		self.m_LeftHanded = false
		self.m_NewItemName = ""
		self.m_ItemTime = 0.0
		self.m_ItemDropTime = -1.0
		
		self.m_CurrentAnimation = "none"
		self.m_LastAnimation = "none"
		self.m_InteractingAnimation = 0
		self.m_InteractingCinematic = nil
		self.m_CameraAnimation = nil
		self.m_AnimationTime = 0
		self.m_InitialCameraRotation = nil
		self.m_FinalCameraRotation = nil
		
		self.m_RaycastData = RaycastData()
		
		self.m_Timer = 0.0
		self.m_TimerRotation = 0.0
		self.m_OrganKeyCount = 1
		self.m_OrganKeyOrder = {"A", "G"}
		--{"A", "B", "C", "D", "E", "F", "G"}
		--table:setn(self.m_OrganKeyOrder, 2)
		
		self.m_CurrentAend = nil
		self.m_Aends = {}
		local l_AendsXML = XMLDocument()
		local l_AendsLoaded = l_AendsXML:load_file("Data\\animation_ends.xml")
		if l_AendsLoaded == 0 then
			local l_Aend = l_AendsXML:first_child_element("aends"):first_child()
			while l_Aend ~= nil do
				local l_AendName = l_Aend:get_name()
				if l_AendName == "aend" then
					self.m_Aends[l_Aend:get_psz_property("name", "")] = l_Aend:get_vect3f_property("pos", Vect3f(0.0,0.0,0.0))
				end
				l_Aend = l_Aend:get_next()
			end
		else
			----utils_log("Animation ends xml not correctly loaded.")
		end
		
		self.m_StateMachine = StateMachine.create()
		self:SetPlayerStateMachine()
		self.m_StateMachine:start()
		
		self.m_PhysXManager:create_character_controller(self.m_Name, g_Height, g_Radius, 90, self.m_RenderableObject:get_position(),"FisicasAux", "Player")
	end
	
	function CPlayer:Destroy()
		utils_log("Delete Player")
		self.m_PhysXManager:remove_actor(self.m_Name)
	end

	function CPlayer:__gc()
		utils_log("Delete Player")
	end
	
	function CPlayer:SetSanity(_amount, _override)
		self.m_Sanity = _amount
		if _override then
			self.m_MaxSanity = _amount
		end
	end
	
	function CPlayer:SetMaxSanity(_amount)
		self.m_MaxSanity = _amount
	end
	
	function CPlayer:ModifySanity(_amount)
		self.m_Sanity = math.max(math.min(self.m_Sanity + _amount, self.m_MaxSanity),0)
		if self.m_Sanity <= 0 then
			utils_log("ESTAS MUERTO!!!")
			g_Engine:set_pause(true)
			m_retry = true
			m_menu = true
			utils_log("ACTIVANDO MENU!!!")
		end
	end
	
	function CPlayer:SetActualLevel(_LevelId)
		g_Engine:get_level_manager():set_actual_level(_LevelId)
		self:SetActualLevelAux(_LevelId)
	end
	function CPlayer:SetActualLevelAux(_LevelId)
		self.m_ActualLevel = _LevelId		
		self.m_CinematicManager = g_Engine:get_level_manager():get_level(self.m_ActualLevel):get_cinematic_manager()		
	end
	
	function CPlayer:GetActualLevel()
		return self.m_ActualLevel
	end
	
	function CPlayer:UpdateSanityEffects(_ElapsedTime)
		--utils_log("Update Sanity")
		local l_Level = g_Engine:get_level_manager():get_level(self.m_ActualLevel)
		local l_LevelPlayer = g_Engine:get_level_manager():get_level("Player")
		
		for i=1, table.maxn(self.m_SanityEffects) do
			l_EffectAux = self.m_SanityEffects[i]
			
			if self.m_Sanity <= l_EffectAux[3] and self.m_Sanity >= l_EffectAux[4] then
				if l_EffectAux[1] == "vortex" then
					--utils_log("vortex")
					l_Material = l_Level:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Value = 0.0
					local l_Previous = l_Material:get_value(1)
					
					if l_EffectAux[2] == "lineal" then
						l_Value = (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])												
					else
						self.m_TimerVortex = self.m_TimerVortex + _ElapsedTime
						
						l_AuxValue = (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])
						l_AuxValueMin = l_AuxValue - l_EffectAux[6]
						l_AuxValueMax = l_AuxValue + l_EffectAux[6]
						
						l_Value = l_AuxValueMin + math.cos(self.m_TimerVortex)*l_EffectAux[6]*2
					end
					
					l_Value = l_Previous + (l_Value-l_Previous)*_ElapsedTime
					l_Material:set_value(1,l_Value)
				elseif l_EffectAux[1] == "stain" then
					--utils_log("stain")
					local l_Layer = l_LevelPlayer:get_layer_manager():get_layer("manchas")
					local l_Mancha = l_Layer:get_resource(l_EffectAux[5])
					local l_ManchaType = l_LevelPlayer:get_manchas_manager():get_resource(l_EffectAux[2])
					
					l_Mancha:set_type(l_ManchaType)
					l_Mancha:set_awake(true)
				elseif l_EffectAux[1] == "vignetting" then
					--utils_log("vignetting")
					l_Material = l_Level:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Value = 0.0
					local l_Previous = l_Material:get_value(1)
					
					if l_EffectAux[2] == "lineal" then
						l_Value = (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])
					end
					
					l_Value = l_Previous + (l_Value-l_Previous)*_ElapsedTime
					l_Material:set_value(1,l_Value)
				elseif l_EffectAux[1] == "fov" then
					--utils_log("fov")
					local CameraControllerManager = g_Engine:get_camera_controller_manager()
					local MainCamera = CameraControllerManager:get_main_camera()
					local l_Fov_Value = l_EffectAux[5] + (l_EffectAux[6] - l_EffectAux[5]) * (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])
					local l_Previous_fov = MainCamera:get_fov()
					
					l_Fov_Value = l_Previous_fov + (l_Fov_Value-l_Previous_fov)*_ElapsedTime					
					MainCamera:set_fov(l_Fov_Value)
					--CameraControllerManager:choose_main_camera("MainCamera")
				elseif l_EffectAux[1] == "velocity" then
					if l_EffectAux[2] == "run" then
						if self.m_CurrentAnimation == "run" then
							self.m_Speed = self.m_DefaultSpeed - ((self.m_DefaultSpeed / 2)* (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4]))
						else
							self.m_Speed = self.m_DefaultSpeed
						end 
					elseif l_EffectAux[2] == "walk" then
						self.m_Speed = self.m_DefaultSpeed - self.m_DefaultSpeed * (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])					
						if self.m_CurrentAnimation == "run" then
							self.m_Speed = self.m_Speed / 2
						end
					end
				end
			else
				if l_EffectAux[1] == "vortex" then
					--utils_log("vortex 2")
					l_Material = l_Level:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Previous = l_Material:get_value(1)
					if l_Previous ~= l_EffectAux[7] then
						local l_Value = l_Previous + (l_EffectAux[7]-l_Previous)*_ElapsedTime
						l_Material:set_value(1, l_Value)
					end
				elseif l_EffectAux[1] == "stain" then
					--utils_log("stain 2")
					local l_Layer = l_LevelPlayer:get_layer_manager():get_layer("manchas")
					local l_Mancha = l_Layer:get_resource(l_EffectAux[5])

					if l_Mancha:get_awake() then
						l_Mancha:set_awake(false)
					end
				elseif l_EffectAux[1] == "vignetting" then
					--utils_log("vignetting 2")
					l_Material = l_Level:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Previous = l_Material:get_value(1)
					if l_Previous ~= l_EffectAux[6] then
						local l_Value = l_Previous + (l_EffectAux[6]-l_Previous)*_ElapsedTime
						l_Material:set_value(1, l_Value)
					end
				elseif l_EffectAux[1] == "fov" then
					--utils_log("fov 2")
					local CameraControllerManager = g_Engine:get_camera_controller_manager()
					local MainCamera = CameraControllerManager:get_main_camera()
					local l_Previous_fov = MainCamera:get_fov()
					
					if l_Previous_fov ~= l_EffectAux[7] then
						local l_Fov_Value = l_Previous_fov + (l_EffectAux[7]-l_Previous_fov)*_ElapsedTime
						MainCamera:set_fov(l_Fov_Value)
						--CameraControllerManager:choose_main_camera("MainCamera")
					end
				end
			end
		end
		--utils_log("End Update Sanity")
	end
	
	--[[function CPlayer:RecoverSanity()
		self.m_Sanity = self.m_MaxSanity
	end]]
	
	function CPlayer:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
		self:UpdateSanityEffects(_ElapsedTime)
	end
	
	function CPlayer:SetPlayerStateMachine()
		
		IdleState = State.create(IdleUpdate)
		IdleState:set_do_first_function(IdleFirst)
		IdleState:set_do_end_function(IdleEnd)
		IdleState:add_condition(IdleToMovingCondition, "Moving")
		IdleState:add_condition(IdleToCrouchingCondition, "Crouching")
		IdleState:add_condition(IdleToJumpingCondition, "Jumping")
		IdleState:add_condition(ANYToFallingCondition, "Falling")
		IdleState:add_condition(ANYToCorrectingCondition, "Correcting")
		IdleState:add_condition(ANYToSingingCondition, "Singing")
		IdleState:add_condition(ANYToDeadCondition, "Dead")
		IdleState:add_condition(ANYToClimbingCondition, "ClimbingStart")
		
		MovingState = State.create(MovingUpdate)
		MovingState:set_do_first_function(MovingFirst)
		MovingState:set_do_end_function(MovingEnd)
		MovingState:add_condition(MovingToIdleCondition, "Idle")
		MovingState:add_condition(ANYToItselfCondition, "Moving")
		MovingState:add_condition(MovingToCrouchingCondition, "Crouching")
		MovingState:add_condition(MovingToJumpingCondition, "Jumping")
		MovingState:add_condition(ANYToFallingCondition, "Falling")
		MovingState:add_condition(ANYToCorrectingCondition, "Correcting")
		MovingState:add_condition(ANYToSingingCondition, "Singing")
		MovingState:add_condition(ANYToDeadCondition, "Dead")
		MovingState:add_condition(ANYToClimbingCondition, "ClimbingStart")
		
		CorrectingState = State.create(CorrectingUpdate)
		CorrectingState:set_do_first_function(CorrectingFirst)
		CorrectingState:set_do_end_function(CorrectingEnd)
		CorrectingState:add_condition(CorrectingToPuzzleCondition, "Puzzle")
		CorrectingState:add_condition(CorrectingToInteractingCondition, "Interacting")
		CorrectingState:add_condition(ANYToDeadCondition, "Dead")

		CrouchingState = State.create(CrouchingUpdate)
		CrouchingState:set_do_first_function(CrouchingFirst)
		CrouchingState:set_do_end_function(CrouchingEnd)
		CrouchingState:add_condition(CrouchingToIdleCondition, "Idle")
		CrouchingState:add_condition(ANYToItselfCondition, "Crouching")
		CrouchingState:add_condition(ANYToFallingCondition, "Falling")
		CrouchingState:add_condition(ANYToCorrectingCondition, "Correcting")
		CrouchingState:add_condition(ANYToDeadCondition, "Dead")

		ClimbingStartState = State.create(ClimbingStartUpdate)
		ClimbingStartState:set_do_first_function(ClimbingStartFirst)
		ClimbingStartState:set_do_end_function(ClimbingStartEnd)
		ClimbingStartState:add_condition(ClimbingStartToClimbingIdleCondition, "ClimbingIdle")
		ClimbingStartState:add_condition(ANYToDeadCondition, "Dead")
		
		ClimbingIdleState = State.create(ClimbingIdleUpdate)
		ClimbingIdleState:set_do_first_function(ClimbingIdleFirst)
		ClimbingIdleState:set_do_end_function(ClimbingIdleEnd)
		ClimbingIdleState:add_condition(ClimbingIdleToInteractingCondition, "Interacting")
		ClimbingIdleState:add_condition(ClimbingIdleToClimbingUpCondition, "ClimbingUp")
		ClimbingIdleState:add_condition(ClimbingIdleToClimbingDownCondition, "ClimbingDown")
		ClimbingIdleState:add_condition(ClimbingIdleToFallingCondition, "Falling")
		ClimbingIdleState:add_condition(ANYToDeadCondition, "Dead")
		
		ClimbingUpState = State.create(ClimbingUpUpdate)
		ClimbingUpState:set_do_first_function(ClimbingUpFirst)
		ClimbingUpState:set_do_end_function(ClimbingUpEnd)
		ClimbingUpState:add_condition(ClimbingUpToClimbingIdleCondition, "ClimbingIdle")
		ClimbingUpState:add_condition(ANYToDeadCondition, "Dead")
		
		ClimbingDownState = State.create(ClimbingDownUpdate)
		ClimbingDownState:set_do_first_function(ClimbingDownFirst)
		ClimbingDownState:set_do_end_function(ClimbingDownEnd)
		ClimbingDownState:add_condition(ClimbingDownToClimbingIdleCondition, "ClimbingIdle")
		ClimbingDownState:add_condition(ClimbingDownToFallingCondition, "Falling")
		ClimbingDownState:add_condition(ANYToDeadCondition, "Dead")
		
		JumpingState = State.create(JumpingUpdate)
		JumpingState:set_do_first_function(JumpingFirst)
		JumpingState:set_do_end_function(JumpingEnd)
		JumpingState:add_condition(ANYToFallingCondition, "Falling")
		JumpingState:add_condition(JumpToIdleCondition, "Idle")
		JumpingState:add_condition(ANYToDeadCondition, "Dead")
		
		FallingState = State.create(FallingUpdate)
		FallingState:set_do_first_function(FallingFirst)
		FallingState:set_do_end_function(FallingEnd)
		FallingState:add_condition(FallingToIdleCondition, "Idle")
		FallingState:add_condition(ANYToDeadCondition, "Dead")
		
		InteractingState = State.create(InteractingUpdate)
		InteractingState:set_do_first_function(InteractingFirst)
		InteractingState:set_do_end_function(InteractingEnd)
		InteractingState:add_condition(InteractingToFallingCondition, "Falling")
		InteractingState:add_condition(ANYToDeadCondition, "Dead")
		
		SingingState = State.create(SingingUpdate)
		SingingState:set_do_first_function(SingingFirst)
		SingingState:set_do_end_function(SingingEnd)
		SingingState:add_condition(SingingToFallingCondition, "Falling")
		SingingState:add_condition(SingingToItselfCondition, "Singing")
		SingingState:add_condition(ANYToDeadCondition, "Dead")
		
		DeadState = State.create(DeadUpdate)
		DeadState:set_do_first_function(DeadFirst)
		DeadState:set_do_end_function(DeadEnd)
		
		PuzzleState = State.create(PuzzleUpdate)
		PuzzleState:set_do_first_function(PuzzleFirst)
		PuzzleState:set_do_end_function(PuzzleEnd)
		PuzzleState:add_condition(PuzzleToFallingCondition, "Falling")
		PuzzleState:add_condition(ANYToDeadCondition, "Dead")
		
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Moving", MovingState)
		self.m_StateMachine:add_state("Correcting", CorrectingState)
		self.m_StateMachine:add_state("Crouching", CrouchingState)
		self.m_StateMachine:add_state("ClimbingStart", ClimbingStartState)
		self.m_StateMachine:add_state("ClimbingIdle", ClimbingIdleState)
		self.m_StateMachine:add_state("ClimbingUp", ClimbingUpState)
		self.m_StateMachine:add_state("ClimbingDown", ClimbingDownState)
		self.m_StateMachine:add_state("Jumping", JumpingState)
		self.m_StateMachine:add_state("Falling", FallingState)
		self.m_StateMachine:add_state("Interacting", InteractingState)
		self.m_StateMachine:add_state("Singing", SingingState)
		self.m_StateMachine:add_state("Dead", DeadState)
		self.m_StateMachine:add_state("Puzzle", PuzzleState)
		
	end	
	
	function CPlayer:SetActiveStateMachineState(name,active)
		self.m_StateMachine:activeState(name,active)
	end

	function CPlayer:SetAnimationCamera(_CameraName, _CopyFirstFrame)
		local l_CameraManager = g_Engine:get_camera_controller_manager()
		local l_FPSCamera = l_CameraManager:get_main_camera()
		local l_AnimatedCamera = l_CameraManager:get_resource(_CameraName)
		if (_CopyFirstFrame) then
			local l_lookAt = l_FPSCamera:get_forward() + l_FPSCamera:get_position()
			l_AnimatedCamera:set_first_key(l_lookAt, l_FPSCamera:get_up(), l_FPSCamera:get_fov())
		end
		local l_CameraKey = l_AnimatedCamera:get_camera_key(0)
		local l_CameraInfo = l_CameraKey:get_camera_info()
		local l_CameraInfoPos = l_CameraInfo:get_eye()
		local l_CameraInfoLookAt = l_CameraInfo:get_look_at()
		l_AnimatedCamera.m_PositionOffsetKey = l_CameraInfoPos
		l_AnimatedCamera.m_PositionOffset = l_FPSCamera:get_position()
		local l_Forward = l_FPSCamera:get_rotation():get_forward_vector()
		l_Forward.y = 0
		l_Forward.x = l_Forward.x * -1.0
		l_Forward:normalize(1)
		local aux = Quatf()
		aux:set_from_fwd_up(l_Forward, Vect3f(0,1,0))
		l_AnimatedCamera.m_RotationOffset = aux:rotation_matrix()
		l_AnimatedCamera:reset_time()
		l_CameraManager:choose_main_camera(_CameraName)
	end
	
	function CPlayer:CalculateCameraPositionRotation(_CameraName, _DesiredPosition)
		local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
		l_AnimatedCamera = l_CameraManager:get_resource(_CameraName)
		local l_CameraInfoPosLength = l_AnimatedCamera:get_camera_key(0):get_camera_info():get_eye()
		l_CameraInfoPosLength.y = 0
		l_CameraInfoPosLength = l_CameraInfoPosLength:length()
		
		local l_AuxPos = self.m_PhysXManager:get_character_controler_pos(self.m_Name)
		local l_auxTarget = l_AuxPos - _DesiredPosition
		l_auxTarget.y = 0
		self.m_Target = l_auxTarget:get_normalized(1) * l_CameraInfoPosLength + _DesiredPosition
		
		self.m_InitialCameraRotation = self.m_CameraController:get_rotation()
		
		local l_CameraDirection = (_DesiredPosition - l_AuxPos)
		l_CameraDirection.y = 0
		l_CameraDirection = l_CameraDirection:get_normalized(1)
						
		local quat_to_turn = Quatf()
		quat_to_turn:set_from_fwd_up(l_CameraDirection, Vect3f(0,1,0))
		self.m_FinalCameraRotation = quat_to_turn
	end
	
	function CPlayer:XZRotate(_ElapsedTime)
		local l_CameraDirection = self.m_CameraController:get_forward()
		l_CameraDirection.y = 0.0
		l_CameraDirection:normalize(1)
		local l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
		local l_Dir = 1
		local l_Difference = math.abs(self.m_TargetYaw-l_OriginYaw)
		local l_DeltaDifference = math.abs(self.m_TargetYaw-l_OriginYaw-_ElapsedTime)
		if l_Difference < l_DeltaDifference then
			l_Dir = -1
		end
		if self.m_TargetYaw == g_PI and l_OriginYaw < 0.0 then
			l_Dir = -1
		end
		local ret = false
		if(l_Difference<= 0.01) then
			ret = true
		else
			self.m_CameraController:add_yaw(l_Dir*_ElapsedTime)
		end
		return ret
	end
	
	function CPlayer:IsFacingTarget(_Target, _Radians, _Distance)
		local l_CameraDirection = self.m_CameraController:get_forward()
		l_CameraDirection.y = 0.0
		l_CameraDirection:normalize(1)
		local l_OriginYaw = math.atan2(l_CameraDirection.z, l_CameraDirection.x)
		local l_Difference = math.abs(self.m_TargetYaw-l_OriginYaw)
		local l_Pos = self.m_PhysXManager:get_character_controler_pos("player")
		l_Pos.y = l_Pos.y - g_TotalHeight
		local ret = false
		utils_log("Distance: "..(l_Pos - _Target):length().." (".._Distance.." needed)")
		if (l_Difference > (g_PI-_Radians)) and (l_Difference < (g_PI+_Radians)) and ((l_Pos - _Target):length() < _Distance) then
			ret = true
		end
		return ret
	end
	
	function CPlayer:ClearTarget()
		self.m_Target = nil
		self.m_TargetYaw = 0.0
		self.m_ForwardCamera = Vect3f(1.0, 0.0, 0.0)
		self.m_UpCamera = Vect3f(0.0, 1.0, 0.0)
		self.m_TargetLookOffset = Vect3f(0.0, 0.0, 0.0)
	end

	function CPlayer:ClearCamera()
		if self.m_CameraAnimation ~= nil then
			l_CameraControllerManager = CUABEngine.get_instance():get_camera_controller_manager()
			l_CameraControllerManager:get_resource(self.m_CameraControllerName):copy_from_key_camera(l_CameraControllerManager:get_main_camera():get_camera_as_info())
			l_CameraControllerManager:choose_main_camera(self.m_CameraControllerName)
		end
		self.m_CameraAnimation = nil
	end

	function CPlayer:ClearCinematic()
		if self.m_InteractingCinematic ~= nil then
			self.m_CinematicManager:get_resource(self.m_InteractingCinematic):stop()
		end
		self.m_InteractingCinematic = nil
	end

	function CPlayer:ClearAend(_Owner)
		if self.m_CurrentAend ~= nil then
			self.m_PhysXManager:character_controller_warp("player", self.m_Aends[self.m_CurrentAend])
			local l_NewControllerPosition = self.m_PhysXManager:get_character_controler_pos("player")
			l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
			_Owner:set_position(l_NewControllerPosition)
		end
		self.m_CurrentAend = nil
	end

	function CPlayer:ClearStates()
		self.m_IsInteracting = false
		self.m_IsPuzzle = false
		self.m_IsClimbing = false
		self.m_IsCorrecting = false
	end
--end

function ANYToItselfCondition(args)
	local lself = args["self"]
	return not (lself.m_LastAnimation == lself.m_CurrentAnimation)
end

function ANYToDeadCondition(args)
	local lself = args["self"]
	return lself.m_IsDead
end
