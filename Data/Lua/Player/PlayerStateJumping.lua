function JumpingFirst(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(5,1.0,0.1)
	l_Player.m_Velocity.y = 4.9
	l_Player.m_SoundManager:play_event(l_Player.m_JumpSoundEvent, l_Player.m_RenderableObject)
end

function JumpingUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	
	utils_log("bu")
	--// Calculate the player speed
	local l_XZDisplacement = Vect2f(l_Player.m_Velocity.x, l_Player.m_Velocity.z)
	local l_ExtraJump = l_XZDisplacement:length()*0.2
	local l_PlayerDisplacement = Vect3f(l_Player.m_Velocity.x * 0.99, l_Player.m_Velocity.y + (l_Player.m_Gravity) * _ElapsedTime, l_Player.m_Velocity.z * 0.99)
	
	utils_log("move charactercontroller")
	--// Move the character controller
	local l_PreviousControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_PreviousControllerPosition.y = l_PreviousControllerPosition.y - g_StandingOffset
	l_Player.m_PhysXManager:character_controller_move("player", l_PlayerDisplacement, _ElapsedTime)
	
	utils_log("Assign to the character the controller's position")
	--// Assign to the character the controller's position
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)
	
	utils_log("Save speed in last update so we can create acceleration")
	--// Save speed in last update so we can create acceleration
	local l_Displacement = l_NewControllerPosition-l_PreviousControllerPosition
	l_Player.m_Velocity = l_Displacement/_ElapsedTime
	
	utils_log("Rotate player to match camera")
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
	
	utils_log("If player has an item, move it.")
	--// If player has an item, move it.
	if l_Player.m_Item ~= nil then
		local l_ObjectPosition
		if l_Player.m_LeftHanded == false then 
			l_ObjectPosition = l_Owner:get_right_object_position()
		else
			l_ObjectPosition = l_Owner:get_left_object_position()
		end
		l_ObjectPosition.x = l_ObjectPosition.x * (-1.0)
		l_ObjectPosition.z = l_ObjectPosition.z * (-1.0)
		l_ObjectPosition = l_Owner:get_rotation():rotated_vector(l_ObjectPosition)
		l_Player.m_Item:set_position(l_ObjectPosition + l_Owner:get_position())
		local l_ObjectRotation = l_Owner:get_left_object_rotation()*l_Owner:get_rotation()
		l_Player.m_Item:set_rotation(l_ObjectRotation)
	end
	
	utils_log("Animate player")
	--// Animate player
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1);
	if l_Displacement.y == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
	utils_log("End")
end

function JumpingEnd(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
end

function JumpToIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Velocity.y == 0
end
