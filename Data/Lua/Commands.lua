dofile("Data\\Lua\\Utils\\GVars.lua")

g_Engine = CUABEngine.get_instance()

function load_level(name)
	local l_LevelManager = g_Engine:get_level_manager()	
	l_LevelManager:load_level(name)
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