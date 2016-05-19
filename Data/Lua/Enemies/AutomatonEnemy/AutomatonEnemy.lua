dofile("Data\\Lua\\Enemies\\Automaton\\AutomatonStateMachine.lua")

function FnOnCreateAutomaton (_owner)
	g_Player.m_PhysXManager:register_material("controllerMaterial", 0.5, 0.5, 0.1)
	g_Player.m_PhysXManager:create_character_controller(g_Player.m_Name, 1.2, 0.3, 0.5, _owner:get_position(),"controllerMaterial", "Enemy")
end

function FnOnDestroyAutomaton ()
		
end

function FnOnUpdateAutomaton (_owner, _ElapsedTime)
	local args = {}
	args["owner"] = _owner
	PlayerStateMachine:update(args, _ElapsedTime)
end

function FnOnRenderAutomaton (_owner, _rm)

end

function FnOnDebugRenderAutomaton (_owner, _rm)
	--g_PhysXManager:render("player", _rm)
end