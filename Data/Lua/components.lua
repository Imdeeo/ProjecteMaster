function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()

	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("porsche")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
end

function FnOnCreateController (_owner)
	local UABEngine = CUABEngine.get_instance()
	local l_physXManager = UABEngine:get_physX_manager()
	l_physXManager:register_material("controllerMaterial", 0.5,0.5,1)
	l_physXManager:create_character_controller("player", 1.5, 1.0, 0.5, _owner:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
	
end

function FnOnUpdateController (_owner, _ElapsedTime)
	local l_InputManager = CInputManager.get_input_manager()

	local Forward = l_InputManager:get_axis("MOVE_CHARACTER")
	local Strafe = l_InputManager:get_axis("STRAFE_CHARACTER")
	
	local l_AddPos = Vect3f(Forward,0,Strafe)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()

	l_physXManager:character_controller_move("player", l_AddPos*4, _ElapsedTime)
	local l_PosCharacterController = l_physXManager:get_character_controler_pos("player")
	_owner:set_position(l_PosCharacterController)

end

function FnOnRenderController(_owner, _rm)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	--l_physXManager:render("player", _rm)
end