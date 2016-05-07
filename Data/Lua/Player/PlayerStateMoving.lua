function MovingFirst(args)
	utils_log("Moving State")
end

function MovingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = CCharacterManager.get_instance():get_resource("player")
	local l_InputManager = CInputManager.get_input_manager()
	local l_PhysXManager = CUABEngine.get_instance():get_physX_manager()
	local l_ForwardMovement = l_InputManager:get_axis("MOVE_FWD")
	local l_StrafeMovement = l_InputManager:get_axis("STRAFE")
	
	if(l_InputManager:is_action_active("JUMP")==true and g_IsJumping==false and g_Velocity.y == 0) then
		g_Velocity.y = 4.9
		g_IsJumping =  true
		g_IsAscending = true
	end
	
	if(g_IsAscending==true and g_Velocity.y<0) then
		g_IsAscending = false
	end
	
	if( g_IsAscending==false and g_Velocity.y == 0)then
		g_IsJumping = false
	end
	
	
	--// Move player forward and laterally
	local l_CameraDirection = l_Player:get_camera_controller():get_forward():get_normalized(1)
	local l_CameraXZDirection = Vect2f(l_ForwardMovement*l_CameraDirection.x,l_ForwardMovement*l_CameraDirection.z)
	local l_CameraXZDirectionOrt = Vect2f(-l_CameraDirection.z*l_StrafeMovement,l_StrafeMovement*l_CameraDirection.x)
	local l_NewPlayerDirection = Vect2f(l_CameraXZDirection.x + l_CameraXZDirectionOrt.x,l_CameraXZDirection.y + l_CameraXZDirectionOrt.y)
	local l_PlayerDisplacement = Vect3f(l_NewPlayerDirection.x*g_Speed,g_Velocity.y+g_Gravity * _ElapsedTime,l_NewPlayerDirection.y*g_Speed)
	
	--// Move the character controller
	local l_PreviousControllerPosition = l_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - 0.9
	l_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - 0.9
	l_Owner:set_position(l_NewControllerPosition)
	
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
	g_Velocity = l_Displacement/_ElapsedTime
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player:get_camera_controller():get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
	
	--// Check if player had displacement, to animate it or not
	local l_X = l_Displacement.x*l_Displacement.x
	--local y = l_Displacement.y*l_Displacement.y
	local l_Y = 0
	local l_Z = l_Displacement.z*l_Displacement.z
	local l_DisplacementModule = math.sqrt(l_X + l_Y + l_Z)
	
	--// Animate player
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1);
	if l_DisplacementModule == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
end

function MovingEnd(args)
	
end

function MovingToIdleCondition()
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()	
		local l_InputManager = CInputManager.get_input_manager()
	return not (l_InputManager:is_action_active("MOVE_FWD") or l_InputManager:is_action_active("MOVE_BACK") or l_InputManager:is_action_active("STRAFE_LEFT") or l_InputManager:is_action_active("STRAFE_RIGHT"))

end