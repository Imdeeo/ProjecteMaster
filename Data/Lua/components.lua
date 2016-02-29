function InitComponents()
	InitBoxMove()
end

function InitBoxMove()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "", "")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
end

function FnOnCreateController ()
	local UABEngine = CUABEngine.get_instance()
	local l_Box=UABEngine:get_layer_manager():get_resource("solid"):get_resource("Box001")
	local l_physXManager = UABEngine:get_physX_manager()
	l_physXManager:register_material("controllerMaterial", 0.5,0.5,1)
	l_physXManager:create_character_controller("player", 1.5, 1, 0.5, l_Box:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
	
end

function FnOnUpdateController ()
	local l_InputManager = CInputManager.get_input_manager()
	local l_AddPos = new Vect3f(1,1,1)

	local Forward = l_InputManager:get_axis("MOVE_CHARACTER")
	--local Strafe = l_InputManager:get_axis("STRAFE_CHARACTER")
	
	--if Strafe != 0 then
		utils_log("uno!")
	--end
	--l_AddPos.y = 
	--[[Forward*(sin(m_Pitch));
	l_AddPos.x=Forward*(cos(m_Yaw))+Strafe*(cos(m_Yaw+3.14159f*0.5f));
	l_AddPos.z=Forward*(sin(m_Yaw))+Strafe*(sin(m_Yaw+3.14159f*0.5f));
	if ((x * x + y * y + z * z) > 0)
	l_AddPos.Normalize();
	l_AddPos *= l_ConstantSpeed;
	move_character_controller(l_AddPos)]]
end