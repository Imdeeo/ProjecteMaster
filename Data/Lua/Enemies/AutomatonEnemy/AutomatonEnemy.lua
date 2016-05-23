dofile("Data\\Lua\\Enemies\\AutomatonEnemy\\AutomatonStateMachine.lua")

function FnOnCreateAutomaton (_owner)
	local l_Enemy = m_CharacterManager.m_Enemics[1]
	l_Enemy.m_PhysXManager:create_character_controller(l_Enemy.m_Name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", "Enemy")
	utils_log("OnCreateFogAutomatonScript")
end

function FnOnDestroyAutomaton ()
		
end

function FnOnUpdateAutomaton (_owner, _ElapsedTime)
	local args = {}
	args["owner"] = _owner
	AutomatonStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderAutomaton (_owner, _rm)

end

function FnOnDebugRenderAutomaton (_owner, _rm)
	--g_PhysXManager:render("enemy", _rm)
end