-- STATES
dofile("Data\\Lua\\Utils\\state_machine.lua")
dofile("Data\\Lua\\Player\\PlayerStateIdle.lua")
dofile("Data\\Lua\\Player\\PlayerStateMoving.lua")
dofile("Data\\Lua\\Player\\PlayerStateCorrecting.lua")
dofile("Data\\Lua\\Player\\PlayerStateCrouching.lua")
dofile("Data\\Lua\\Player\\PlayerStateClimbing.lua")
dofile("Data\\Lua\\Player\\PlayerStateJumping.lua")
dofile("Data\\Lua\\Player\\PlayerStateFalling.lua")
dofile("Data\\Lua\\Player\\PlayerStateInteracting.lua")
dofile("Data\\Lua\\Player\\PlayerStateDead.lua")

class 'CPlayer' (CLUAComponent)
	function CPlayer:__init(_TreeNode)
		self.m_AlreadyInitialized = false
		local UABEngine = CUABEngine.get_instance()
		self.m_Name = _TreeNode:get_psz_property("name", "", false)
		self.m_LayerName = _TreeNode:get_psz_property("layer", "", false)
		self.m_RenderableObjectName = _TreeNode:get_psz_property("renderable_object", "", false)
		self.m_RenderableObject = UABEngine:get_layer_manager():get_resource(self.m_LayerName):get_resource(self.m_RenderableObjectName)
		utils_log("name: "..self.m_RenderableObject.name)
		CLUAComponent.__init(self,self.m_Name, self.m_RenderableObject)
	
		self.m_CameraControllerName= _TreeNode:get_psz_property("camera_controller", "", false)
		self.m_CameraController = UABEngine:get_camera_controller_manager():get_resource(self.m_CameraControllerName)
		self.m_LuaCommand = _TreeNode:get_psz_property("lua_command", "", false)
		
		self.m_SoundManager = CUABEngine.get_instance():get_sound_manager()
		if self.m_AlreadyInitialized then
			-- unregister old speaker before assigning new renderable object
			self.m_SoundManager:unregister_speaker(self.m_RenderableObject)
		end
		
		self.m_InputManager = CInputManager.get_input_manager()
		self.m_PhysXManager = CUABEngine.get_instance():get_physX_manager()
		if(not UABEngine:get_lua_reloaded())then
			self.m_SoundManager:register_speaker(self.m_RenderableObject)
		end
		self.m_JumpSoundEvent = SoundEvent()
		self.m_JumpSoundEvent.event_name = "Jump"
		self.m_Velocity = Vect3f(0, 0, 0)
		self.m_Gravity = -9.81
		self.m_Speed = 5
		self.m_Sanity = 100
		self.m_MaxSanity = 100
		
		self.m_IsCorrecting = false
		self.m_IsClimbing = false
		self.m_IsInteracting = false
		self.m_Target = nil
		self.m_TargetOffset = Vect3f(0, 0, 0)
		
		self.m_StateMachine = StateMachine.create()
		self:SetPlayerStateMachine()
		self.m_StateMachine:start()
		if(not UABEngine:get_lua_reloaded())then
			self.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
			self.m_PhysXManager:create_character_controller(self.m_Name, 1.2, 0.3, 0.5, self.m_RenderableObject:get_position(),"controllerMaterial", "Player")
		end

		self.m_AlreadyInitialized = true
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
	
	function CPlayer:RecoverSanity()
		self.m_Sanity = self.m_MaxSanity
	end
	
	--[[function CPlayer:ModifySanity(_amount, _override)
		self.m_Sanity = self.m_Sanity + _amount
		if _override and self.m_Sanity > self.m_MaxSanity then
			self.m_MaxSanity = self.m_Sanity
		else
	end
	
	function CPlayer:GainSanity()
		self.m_Sanity = self.m_Sanity + 10
		if self.m_Sanity > self.m_MaxSanity then
			self.m_Sanity = self.m_MaxSanity
		end
	end
	
	function CPlayer:LoseSanity()
		self.m_Sanity = self.m_Sanity - 10
		if self.m_Sanity < 0 then
			self.m_Sanity = 0
		end
	end]]
	
	function CPlayer:Update(_ElapsedTime)
		local args = {}
		args["owner"] = self.m_RenderableObject
		args["self"] = self
		self.m_StateMachine:update(args, _ElapsedTime)
		--PlayerStateMachine:update(args, _ElapsedTime)
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
		
		MovingState = State.create(MovingUpdate)
		MovingState:set_do_first_function(MovingFirst)
		MovingState:set_do_end_function(MovingEnd)
		MovingState:add_condition(MovingToIdleCondition, "Idle")
		MovingState:add_condition(MovingToCrouchingCondition, "Crouching")
		MovingState:add_condition(MovingToJumpingCondition, "Jumping")
		MovingState:add_condition(ANYToFallingCondition, "Falling")
		MovingState:add_condition(ANYToCorrectingCondition, "Correcting")
		
		CorrectingState = State.create(CorrectingUpdate)
		CorrectingState:set_do_first_function(CorrectingFirst)
		CorrectingState:set_do_end_function(CorrectingEnd)
		CorrectingState:add_condition(CorrectingToClimbingCondition, "Climbing")
		CorrectingState:add_condition(CorrectingToInteractingCondition, "Interacting")

		CrouchingState = State.create(CrouchingUpdate)
		CrouchingState:set_do_first_function(CrouchingFirst)
		CrouchingState:set_do_end_function(CrouchingEnd)
		CrouchingState:add_condition(CrouchingToIdleCondition, "Idle")
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
		self.m_StateMachine:add_state("Dead", DeadState)
		
		utils_log("End Set PlayerStateMachine")
	end	
--end