dofile("Data\\Lua\\Enemies\\FogEnemy\\FogStateMachine.lua")

function FnOnCreateFogAutomaton (_owner)
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	utils_log(l_Enemy.m_Name)
	l_Enemy.m_PhysXManager:create_character_controller(l_Enemy.m_Name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", "Enemy")
	utils_log("OnCreateFogAutomatonScript")
end

function FnOnDestroyFogAutomaton ()
	
end

function FnOnUpdateFogAutomaton (_owner, _ElapsedTime)
	local args = {}
	args["owner"] = _owner
	FogAutomatonStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderFogAutomaton (_owner, _rm)

end