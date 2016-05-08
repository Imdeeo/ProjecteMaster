function IdleFirst(args)
	utils_log("IdleFirst")
end

function IdleUpdate(args, _ElapsedTime)
	local l_Owner = args["owner"]
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = g_Player:get_camera_controller():get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
end

function IdleEnd(args)
	utils_log("IdleEnd")
end

function IdleToMovingCondition()
	return (g_InputManager:is_action_active("MOVE_FWD") or g_InputManager:is_action_active("MOVE_BACK") or g_InputManager:is_action_active("STRAFE_LEFT") or g_InputManager:is_action_active("STRAFE_RIGHT"))
end

function IdleToJumpingCondition()
	return g_InputManager:is_action_active("JUMP")
end