function IdleFirst(args)
	utils_log("Idle State")
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player:get_camera_controller():get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function IdleEnd(args)
	
end

function IdleToMovingCondition()
	local l_InputManager = CInputManager.get_input_manager()
	return (l_InputManager:is_action_active("MOVE_FWD") or l_InputManager:is_action_active("MOVE_BACK") or l_InputManager:is_action_active("STRAFE_LEFT") or l_InputManager:is_action_active("STRAFE_RIGHT"))
end