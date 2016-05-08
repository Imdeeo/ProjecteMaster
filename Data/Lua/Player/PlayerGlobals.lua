function InitGlobals()
	g_Velocity = Vect3f(0,0,0)
	g_Gravity = -9.81
	g_Speed = 10
	g_IsJumping = false
	g_IsAscending = false
	g_Player = CCharacterManager.get_instance():get_resource("player")
	g_InputManager = CInputManager.get_input_manager()
	g_PhysXManager = CUABEngine.get_instance():get_physX_manager()
end