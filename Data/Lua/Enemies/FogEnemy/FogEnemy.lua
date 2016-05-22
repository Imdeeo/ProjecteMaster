dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateMachine.lua")

function FnOnCreateFogAutomaton (_owner)
	--l_Enemy = m_CharacterManager.m_Enemies[1]
	--utils_log("pene")
	--utils_log(l_Enemy.name)
--	l_Enemy.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
--	l_Enemy.m_PhysXManager:create_character_controller(l_Enemy.m_Name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", "Enemy")
	utils_log("OnCreateFogAutomatonScript")
end

function FnOnDestroyFogAutomaton ()
		
end

function FnOnUpdateFogAutomaton (_owner, _ElapsedTime)
	utils_log("OnUpdateFogAutomatonScript")
	local args = {}
	args["owner"] = _owner
	FogAutomatonStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderFogAutomaton (_owner, _rm)

end