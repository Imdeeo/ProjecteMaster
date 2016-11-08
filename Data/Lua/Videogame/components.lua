function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()

	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Bruja")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "FnOnDebugRender")
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

	l_physXManager:character_controller_move("player", l_AddPos*2, _ElapsedTime)
	local l_PosCharacterController = l_physXManager:get_character_controler_pos("player")
	_owner:set_position(l_PosCharacterController)
	
	local x = l_AddPos.x*l_AddPos.x
	local y = l_AddPos.y*l_AddPos.y
	local z = l_AddPos.z*l_AddPos.z
	local result = math.sqrt(x+y+z)
	
	_owner:clear_cycle(_owner:get_actual_cycle_animation(),0.1);
	if result == 0 then		
		_owner:blend_cycle(1,1.0,0.1);
	else
		_owner:blend_cycle(0,1.,0.1);
	end
	
	local jump = l_InputManager:is_action_active("MONSTER_RUN")
	
	--_owner:execute_action(6,0.1,0.1);
	
	
	----utils_log("no peta");

	--[[/*if(CInputManager::GetInputManager()->IsActionActive("MONSTER_IDLE"))
	{
		if(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")!=nullptr)
		{
			if(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->IsActionAnimationActive(2))
			{
				((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					RemoveAction(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					GetActualActionAnimation());
			}
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualCycleAnimation(),0.1f);
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(0,1.f,0.1f);
		}
	}
	if(CInputManager::GetInputManager()->IsActionActive("MONSTER_RUN"))
	{
		if(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")!=nullptr)
		{
			if(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->IsActionAnimationActive(2))
			{
				((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
					RemoveAction(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
				GetActualActionAnimation());
			}
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			ClearCycle(((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->
			GetActualCycleAnimation(),0.1f);
			((CAnimatedInstanceModel*)(UABEngine.GetRenderableObjectsManager()->GetResource("Bot001")))->BlendCycle(1,1.f,0.1f);
		}
	}]]
end

function FnOnRenderController(_owner, _rm)

end

function FnOnDebugRender(_owner, _rm)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	--l_physXManager:render("player", _rm)
end