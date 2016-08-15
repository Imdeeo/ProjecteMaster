--// Global Variables
dofile("Data\\Lua\\Player\\GVars.lua")
--// RaycastData
dofile("Data\\Lua\\Raycast.lua")
--// StateMachine
dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Player\\PlayerStateIdle.lua")
dofile("Data\\Lua\\Player\\PlayerStateMoving.lua")
dofile("Data\\Lua\\Player\\PlayerStateCorrecting.lua")
dofile("Data\\Lua\\Player\\PlayerStateCrouching.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbing.lua")
dofile("Data\\Lua\\Player\\PlayerStateJumping.lua")
dofile("Data\\Lua\\Player\\PlayerStateFalling.lua")
dofile("Data\\Lua\\Player\\PlayerStateInteracting.lua")
dofile("Data\\Lua\\Player\\PlayerStateSinging.lua")
dofile("Data\\Lua\\Player\\PlayerStateDead.lua")

--Bone ID: 0. Name: CATRigHub001
--Bone ID: 1. Name: CATRigLArm1
--Bone ID: 2. Name: CATRigLArm2
--Bone ID: 3. Name: CATRigLArmPalm
--Bone ID: 4. Name: CATRigLArmDigit11
--Bone ID: 5. Name: CATRigLArmDigit12
--Bone ID: 6. Name: CATRigLArmDigit13
--Bone ID: 7. Name: CATRigLArmDigit21
--Bone ID: 8. Name: CATRigLArmDigit22
--Bone ID: 9. Name: CATRigLArmDigit23
--Bone ID: 10. Name: CATRigLArmDigit31
--Bone ID: 11. Name: CATRigLArmDigit32
--Bone ID: 12. Name: CATRigLArmDigit33
--Bone ID: 13. Name: CATRigLArmDigit41
--Bone ID: 14. Name: CATRigLArmDigit42
--Bone ID: 15. Name: CATRigLArmDigit43
--Bone ID: 16. Name: CATRigLArmDigit51
--Bone ID: 17. Name: CATRigLArmDigit52
--Bone ID: 18. Name: CATRigLArmDigit53
--Bone ID: 19. Name: CATRigObjeto
--Bone ID: 20. Name: CATRigRArm1
--Bone ID: 21. Name: CATRigRArm2
--Bone ID: 22. Name: CATRigRArmPalm
--Bone ID: 23. Name: CATRigRArmDigit11
--Bone ID: 24. Name: CATRigRArmDigit12
--Bone ID: 25. Name: CATRigRArmDigit13
--Bone ID: 26. Name: CATRigRArmDigit21
--Bone ID: 27. Name: CATRigRArmDigit22
--Bone ID: 28. Name: CATRigRArmDigit23
--Bone ID: 29. Name: CATRigRArmDigit31
--Bone ID: 30. Name: CATRigRArmDigit32
--Bone ID: 31. Name: CATRigRArmDigit33
--Bone ID: 32. Name: CATRigRArmDigit41
--Bone ID: 33. Name: CATRigRArmDigit42
--Bone ID: 34. Name: CATRigRArmDigit43
--Bone ID: 35. Name: CATRigRArmDigit51
--Bone ID: 36. Name: CATRigRArmDigit52
--Bone ID: 37. Name: CATRigRArmDigit53
--Bone ID: 38. Name: CATRigObjeto

class 'CPlayer' (CLUAComponent)
	function CPlayer:__init(_TreeNode)
		self.m_AlreadyInitialized = false
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		CLUAComponent.__init(self, self.m_Name, self.m_RenderableObject)
	
		self.m_CameraControllerName= _TreeNode:get_psz_property("camera_controller", "", false)
		self.m_CameraController = UABEngine:get_camera_controller_manager():get_resource(self.m_CameraControllerName)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		
		-- distance, time, sanity amount
		self.m_SanityEffects = {}
		for i = 0, _TreeNode:get_num_children()-1 do
			local l_Param = _TreeNode:get_child(i)
			if l_Param:get_name() == "vortex" then
				l_VortexEffects = {}
				table.insert(l_VortexEffects, "vortex")
				table.insert(l_VortexEffects, l_Param:get_psz_property("type","",false))
				table.insert(l_VortexEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_VortexEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(l_VortexEffects, l_Param:get_psz_property("material","",false))
				table.insert(l_VortexEffects, l_Param:get_float_property("variation",0.0,false))
				table.insert(l_VortexEffects, l_Param:get_float_property("default_value",0.0,false))
				table.insert(self.m_SanityEffects, l_VortexEffects)
			elseif l_Param:get_name() == "stain" then
				l_StainEffects = {}
				table.insert(l_StainEffects, "stain")
				table.insert(l_StainEffects, l_Param:get_psz_property("type","",false))
				table.insert(l_StainEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_StainEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(l_StainEffects, l_Param:get_psz_property("renderable_object","",false))
				table.insert(self.m_SanityEffects, l_StainEffects)
			elseif l_Param:get_name() == "vignetting" then
				l_VignettinEffects = {}
				table.insert(l_VignettinEffects, "vignetting")
				table.insert(l_VignettinEffects, l_Param:get_psz_property("type","",false))
				table.insert(l_VignettinEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_VignettinEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(l_VignettinEffects, l_Param:get_psz_property("material","",false))
				table.insert(l_VignettinEffects, l_Param:get_float_property("default_value",0.0,false))
				table.insert(self.m_SanityEffects, l_VignettinEffects)
			elseif l_Param:get_name() == "fov" then
				l_FovEffects = {}
				table.insert(l_FovEffects, "fov")
				table.insert(l_FovEffects, l_Param:get_psz_property("type","",false))
				table.insert(l_FovEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_FovEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(l_FovEffects, l_Param:get_float_property("start_value",0.0,false))
				table.insert(l_FovEffects, l_Param:get_float_property("end_value",0.0,false))
				table.insert(l_FovEffects, l_Param:get_float_property("default_value",0.0,false))
				table.insert(self.m_SanityEffects, l_FovEffects)
			elseif l_Param:get_name() == "velocity" then
				l_VelocityEffects = {}
				table.insert(l_VelocityEffects, "velocity")
				table.insert(l_VelocityEffects, l_Param:get_psz_property("type","",false))				
				table.insert(l_VelocityEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_VelocityEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(self.m_SanityEffects, l_VelocityEffects)
			--[[elseif l_Param:get_name() == "control" then
				l_ControlEffects = {}
				table.insert(l_ControlEffects, "control")
				table.insert(l_ControlEffects, l_Param:get_psz_property("type","",false))
				table.insert(l_ControlEffects, l_Param:get_float_property("start",0.0,false))
				table.insert(l_ControlEffects, l_Param:get_float_property("end",0.0,false))
				table.insert(self.m_SanityEffects, l_ControlEffects)]]
			end
		end
		utils_log("PARAMETROS CORDURA LEIDOS CORRECTAMENTE!!!")
		
		self.m_SoundManager = UABEngine:get_sound_manager()
		if self.m_AlreadyInitialized then
			-- unregister old speaker before assigning new renderable object
			self.m_SoundManager:unregister_speaker(self.m_RenderableObject)
		end
		
		self.m_CinematicManager = UABEngine:get_cinematic_manager()
		self.m_InputManager = UABEngine:get_input_manager()
		self.m_PhysXManager = UABEngine:get_physX_manager()
		if(not UABEngine:get_lua_reloaded())then
			self.m_SoundManager:register_speaker(self.m_RenderableObject)
		end
		
		self.m_JumpSoundEvent = SoundEvent()
		self.m_JumpSoundEvent.event_name = "Jump"
		self.m_Velocity = Vect3f(0.0, 0.0, 0.0)
		self.m_Gravity = -9.81
		self.m_Speed = 5.0
		self.m_DefaultSpeed = 5.0
		self.m_Sanity = 100.0
		self.m_MaxSanity = 100.0
		self.m_TimerVortex = 0
		
		self.m_IsSinging = false
		self.m_IsWindedUp = false
		self.m_IsCorrecting = false
		self.m_IsClimbing = false
		self.m_IsInteracting = false
		
		self.m_Target = nil
		self.m_TargetOffset = Vect3f(1.0, 0.0, 0.0)
		self.m_Item = nil --CUABEngine.get_instance():get_layer_manager():get_resource("solid"):get_resource("LlaveRecibidor")
		self.m_ItemName = "" --"LlaveRecibidor"
		self.m_ItemTime = 0
		
		self.m_CurrentAnimation = "none"
		self.m_LastAnimation = "none"
		self.m_InteractingAnimation = 0
		self.m_InteractingCinematic = nil
		self.m_CameraAnimation = nil
		self.m_AnimationTime = 0
		
		self.m_RaycastData = RaycastData()
		
		self.m_StateMachine = StateMachine.create()
		self:SetPlayerStateMachine()
		self.m_StateMachine:start()
		
		if(not UABEngine:get_lua_reloaded())then
			self.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
			self.m_PhysXManager:create_character_controller(self.m_Name, g_Height, g_Radius, 0.5, self.m_RenderableObject:get_position(),"controllerMaterial", "Player")
		end

		self.m_AlreadyInitialized = true
		utils_log("Player init finished")
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
	end
	
	function CPlayer:UpdateSanityEffects(_ElapsedTime)
		local UABEngine = CUABEngine.get_instance()
		
		for i=1, table.maxn(self.m_SanityEffects) do
			l_EffectAux = self.m_SanityEffects[i]
			
			if self.m_Sanity <= l_EffectAux[3] and self.m_Sanity >= l_EffectAux[4] then
				if l_EffectAux[1] == "vortex" then
					l_Material = UABEngine:get_material_manager():get_resource(l_EffectAux[5])
					
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
					local l_Layer = UABEngine:get_layer_manager():get_layer("manchas")
					local l_Mancha = l_Layer:get_resource(l_EffectAux[5])
					local l_ManchaType = UABEngine:get_manchas_manager():get_resource(l_EffectAux[2])
					
					l_Mancha:set_type(l_ManchaType)
					l_Mancha:set_awake(true)
				elseif l_EffectAux[1] == "vignetting" then
					l_Material = UABEngine:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Value = 0.0
					local l_Previous = l_Material:get_value(1)
					
					if l_EffectAux[2] == "lineal" then
						l_Value = (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])
					end
					
					l_Value = l_Previous + (l_Value-l_Previous)*_ElapsedTime
					l_Material:set_value(1,l_Value)
				elseif l_EffectAux[1] == "fov" then
					local CameraControllerManager = UABEngine:get_camera_controller_manager()
					local MainCamera = CameraControllerManager:get_main_camera()
					local l_Fov_Value = l_EffectAux[5] + (l_EffectAux[6] - l_EffectAux[5]) * (l_EffectAux[3] - self.m_Sanity) / (l_EffectAux[3] - l_EffectAux[4])
					local l_Previous_fov = MainCamera:get_fov()
					
					l_Fov_Value = l_Previous_fov + (l_Fov_Value-l_Previous_fov)*_ElapsedTime					
					MainCamera:set_fov(l_Fov_Value)
					CameraControllerManager:choose_main_camera("MainCamera")
				elseif l_EffectAux[1] == "velocity" then
					utils_log("TYPE: "..l_EffectAux[2])
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
					--utils_log("STATE: "..self.m_CurrentAnimation..", SPEED: "..self.m_Speed)
				--elseif l_EffectAux[1] == "control" then					
				end
			else
				if l_EffectAux[1] == "vortex" then
					l_Material = UABEngine:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Previous = l_Material:get_value(1)
					if l_Previous ~= l_EffectAux[7] then
						local l_Value = l_Previous + (l_EffectAux[7]-l_Previous)*_ElapsedTime
						l_Material:set_value(1, l_Value)
					end
				elseif l_EffectAux[1] == "stain" then
					local l_Layer = UABEngine:get_layer_manager():get_layer("manchas")
					local l_Mancha = l_Layer:get_resource(l_EffectAux[5])

					if l_Mancha:get_awake() then
						l_Mancha:set_awake(false)
					end
				elseif l_EffectAux[1] == "vignetting" then
					l_Material = UABEngine:get_material_manager():get_resource(l_EffectAux[5])
					
					local l_Previous = l_Material:get_value(1)
					if l_Previous ~= l_EffectAux[6] then
						local l_Value = l_Previous + (l_EffectAux[6]-l_Previous)*_ElapsedTime
						l_Material:set_value(1, l_Value)
					end
				elseif l_EffectAux[1] == "fov" then
					local CameraControllerManager = UABEngine:get_camera_controller_manager()
					local MainCamera = CameraControllerManager:get_main_camera()
					local l_Previous_fov = MainCamera:get_fov()
					
					if l_Previous_fov ~= l_EffectAux[7] then
						local l_Fov_Value = l_Previous_fov + (l_EffectAux[7]-l_Previous_fov)*_ElapsedTime
						MainCamera:set_fov(l_Fov_Value)
						CameraControllerManager:choose_main_camera("MainCamera")
					end
				end
			end
		end
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
		utils_log("Start Set PlayerStateMachine")
		
		IdleState = State.create(IdleUpdate)
		IdleState:set_do_first_function(IdleFirst)
		IdleState:set_do_end_function(IdleEnd)
		IdleState:add_condition(IdleToMovingCondition, "Moving")
		IdleState:add_condition(IdleToCrouchingCondition, "Crouching")
		IdleState:add_condition(IdleToJumpingCondition, "Jumping")
		IdleState:add_condition(ANYToFallingCondition, "Falling")
		IdleState:add_condition(ANYToCorrectingCondition, "Correcting")
		IdleState:add_condition(ANYToSingingCondition, "Singing")
		
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
		
		CorrectingState = State.create(CorrectingUpdate)
		CorrectingState:set_do_first_function(CorrectingFirst)
		CorrectingState:set_do_end_function(CorrectingEnd)
		CorrectingState:add_condition(CorrectingToClimbingCondition, "Climbing")
		CorrectingState:add_condition(CorrectingToInteractingCondition, "Interacting")

		CrouchingState = State.create(CrouchingUpdate)
		CrouchingState:set_do_first_function(CrouchingFirst)
		CrouchingState:set_do_end_function(CrouchingEnd)
		CrouchingState:add_condition(CrouchingToIdleCondition, "Idle")
		CrouchingState:add_condition(ANYToItselfCondition, "Crouching")
		CrouchingState:add_condition(ANYToFallingCondition, "Falling")
		CrouchingState:add_condition(ANYToCorrectingCondition, "Correcting")
		
		ClimbingState = State.create(ClimbingUpdate)
		ClimbingState:set_do_first_function(ClimbingFirst)
		ClimbingState:set_do_end_function(ClimbingEnd)
		ClimbingState:add_condition(ClimbingToFallingCondition, "Falling")
		
		JumpingState = State.create(JumpingUpdate)
		JumpingState:set_do_first_function(JumpingFirst)
		JumpingState:set_do_end_function(JumpingEnd)
		JumpingState:add_condition(ANYToFallingCondition, "Falling")
		
		FallingState = State.create(FallingUpdate)
		FallingState:set_do_first_function(FallingFirst)
		FallingState:set_do_end_function(FallingEnd)
		FallingState:add_condition(FallingToIdleCondition, "Idle")
		
		InteractingState = State.create(InteractingUpdate)
		InteractingState:set_do_first_function(InteractingFirst)
		InteractingState:set_do_end_function(InteractingEnd)
		InteractingState:add_condition(InteractingToFallingCondition, "Falling")
		
		SingingState = State.create(SingingUpdate)
		SingingState:set_do_first_function(SingingFirst)
		SingingState:set_do_end_function(SingingEnd)
		SingingState:add_condition(SingingToFallingCondition, "Falling")
		SingingState:add_condition(SingingToItselfCondition, "Singing")
		
		DeadState = State.create(DeadUpdate)
		DeadState:set_do_first_function(DeadFirst)
		DeadState:set_do_end_function(DeadEnd)
		
		self.m_StateMachine:add_state("Idle", IdleState)
		self.m_StateMachine:add_state("Moving", MovingState)
		self.m_StateMachine:add_state("Correcting", CorrectingState)
		self.m_StateMachine:add_state("Crouching", CrouchingState)
		self.m_StateMachine:add_state("Climbing", ClimbingState)
		self.m_StateMachine:add_state("Jumping", JumpingState)
		self.m_StateMachine:add_state("Falling", FallingState)
		self.m_StateMachine:add_state("Interacting", InteractingState)
		self.m_StateMachine:add_state("Singing", SingingState)
		self.m_StateMachine:add_state("Dead", DeadState)
		
		utils_log("End Set PlayerStateMachine")
	end	
--end

function ANYToItselfCondition(args)
	local l_Player = args["self"]
	return not (l_Player.m_LastAnimation == l_Player.m_CurrentAnimation)
end
