dofile("Data\\Lua\\Utils\\GVars.lua")

g_Engine = CUABEngine.get_instance()

function load_level(name)
	local l_LevelManager = g_Engine:get_level_manager()	
	l_LevelManager:load_level(name)
end

function unload_level(name)
	local l_LevelManager = g_Engine:get_level_manager()	
	l_LevelManager:unload_level(name)
end

function set_commands_level(name)
	local l_LevelManager = g_Engine:get_level_manager()	
	l_LevelManager:choose_scene_command_level(name)
end

function activate_gravity()
	g_Player:SetActiveStateMachineState("Jumping",true)
	g_Player:SetActiveStateMachineState("Falling",true)
	g_Player.m_Gravity = g_Gravity
end

function deactivate_gravity()
	g_Player:SetActiveStateMachineState("Jumping",false)
	g_Player:SetActiveStateMachineState("Falling",false)
	g_Player.m_Gravity = 0
end

function change_object_level(level,newlevel,layer,object)
	utils_log("Change Object Level")
	g_Engine:get_level_manager():change_object_level(level,newlevel,layer,object)
end

function activate_player_collisions()
	g_Engine:get_physX_manager():change_rigid_dynamic_actor_group("player","Player")
end

function deactivate_player_collisions()
	g_Engine:get_physX_manager():change_rigid_dynamic_actor_group("player","NoCollision")
end

function activate_player_physx()
	activate_gravity()
	activate_player_collisions()
end

function deactivate_player_physx()
	deactivate_gravity()
	deactivate_player_collisions()
end

function rc()
	local UABEngine = CUABEngine.get_instance()
	UABEngine:get_camera_controller_manager():reload()
end

function test()
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	local l_FPSCamera = l_CameraManager:get_main_camera()
	local l_AnimatedCamera = l_CameraManager:get_resource("JaheemDies")
	local l_CameraKey = l_AnimatedCamera:get_camera_key(0)
	local l_CameraInfo = l_CameraKey:get_camera_info()
	local l_CameraInfoPos = l_CameraInfo:get_eye()
	local l_CameraInfoLookAt = l_CameraInfo:get_look_at()
	l_AnimatedCamera.m_PositionOffsetKey = l_CameraInfoPos
	l_AnimatedCamera.m_PositionOffset = l_FPSCamera:get_position()
	l_AnimatedCamera.m_PositionOffset.y = 1.8
	local l_Forward = l_FPSCamera:get_rotation():get_forward_vector()
	l_Forward.y = 0
	l_Forward.x = l_Forward.x * -1.0
	l_Forward:normalize(1)
	utils_log_v3(l_Forward)
	local aux = Quatf()
	aux:set_from_fwd_up(l_Forward, Vect3f(0,1,0))
	l_AnimatedCamera.m_RotationOffset = aux:rotation_matrix()
	--l_AnimatedCamera:set_first_key(l_FPSCamera:get_forward(), l_FPSCamera:get_position(), l_FPSCamera:get_up(), l_FPSCamera:get_fov())
	l_AnimatedCamera:reset_time()
	l_CameraManager:choose_main_camera("JaheemDies")
end

function test2()
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	l_CameraManager:choose_main_camera("MainCamera")
end

function ja()
	local l_CameraManager = CUABEngine.get_instance():get_camera_controller_manager()
	local l_FPSCamera = l_CameraManager:get_main_camera()
	utils_log_v3(l_FPSCamera:get_position())
end

function asd()
	g_Engine:set_time_scale(0.2)
end

function dsa()
	g_Engine:set_time_scale(1)
end

function rot()
	utils_log_q(g_Player.m_RenderableObject:get_rotation())
end