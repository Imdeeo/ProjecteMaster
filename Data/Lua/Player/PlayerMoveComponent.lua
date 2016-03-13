cct_velocity = Vect3f(0,0,0)

function InitPlayerMove()
	local UABEngine = CUABEngine.get_instance()

	local l_Player = CCharacterManager.get_instance():get_resource("player")
	local l_Box=l_Player:get_renderable_object()
	local l_Component=l_Box:get_component_manager():get_resource("ScriptedComponent")
	
	if l_Component==nil then
		local l_Component=create_scripted_component("ScriptedComponent", l_Box, "FnOnCreateController","FnOnDestroyController", "FnOnUpdateController", "FnOnRenderController", "FnOnDebugRender")
		l_Box:get_component_manager():add_resource("ScriptedComponent", l_Component)
	end
end

function FnOnCreateController (_owner)
	local UABEngine = CUABEngine.get_instance()
	local l_physXManager = UABEngine:get_physX_manager()
	local l_Player = CCharacterManager.get_instance():get_resource("player")
	l_physXManager:register_material("controllerMaterial", 0.5,0.5,1)
	l_physXManager:create_character_controller(l_Player.name, 1.5, 0.3, 0.5, _owner:get_position(),"controllerMaterial", 1)
end

function FnOnDestroyController ()
	
end

--gravity = Vect3f(0,-9.81,0)
gravity = -9.81
--gravity = 0
is_jumping = false
is_ascending = false

function FnOnUpdateController (_owner, _ElapsedTime)
	
	local l_Player = CCharacterManager.get_instance():get_resource("player")
	
	local l_InputManager = CInputManager.get_input_manager()
	local Forward = l_InputManager:get_axis("MOVE_FWD")
	local Strafe = l_InputManager:get_axis("STRAFE")
	
	if(l_InputManager:is_action_active("JUMP")==true and is_jumping==false and cct_velocity.y == 0) then
		cct_velocity.y = 10
		is_jumping =  true
		is_ascending = true
	end
	
	if(is_ascending==true and cct_velocity.y<0) then
		is_ascending = false
	end
	
	if( is_ascending==false and cct_velocity.y == 0)then
		is_jumping = false
	end
	
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	
	local player_camera_direction = l_Player:get_camera_controller():get_direction():get_normalized(1)
	
	local player_camera_direction_xz = Vect2f(Forward*player_camera_direction.x,Forward*player_camera_direction.z)
	local player_camera_direction_xz_ort = Vect2f(-player_camera_direction.z*Strafe,Strafe*player_camera_direction.x)
	
	local final_direction = Vect2f(player_camera_direction_xz.x + player_camera_direction_xz_ort.x,player_camera_direction_xz.y + player_camera_direction_xz_ort.y)
		
	local l_velocity = 10
	local l_AddPos = Vect3f(final_direction.x*l_velocity,cct_velocity.y+gravity * _ElapsedTime,final_direction.y*l_velocity)
	--cct_velocity = cct_velocity + 
	
	local l_velocity = 5
	
	--utils_log("Position x: "..l_AddPos.x..",y: "..l_AddPos.y..",z: "..l_AddPos.z)
	
	local l_PrevPosCharacterController = l_physXManager:get_character_controler_pos("player")
	l_physXManager:character_controller_move("player", l_AddPos, _ElapsedTime)
	local l_PostPosCharacterController = l_physXManager:get_character_controler_pos("player")
	_owner:set_position(l_PostPosCharacterController)
	local l_desplacamiento = l_PostPosCharacterController-l_PrevPosCharacterController
	cct_velocity = l_desplacamiento/_ElapsedTime
	--utils_log("Position x: "..l_PosCharacterController.x..",y: "..l_PosCharacterController.y..",z: "..l_PosCharacterController.z)
	
	local final_rotation = Quatf(0,0,0,1)
	--_owner:set_rotation(final_rotation:set_from_scaled_axis(player_camera_direction))
	
	--local yaw = _owner:get_yaw()
	--local dir = Vect3f(math.cos(yaw),0,math.sin(yaw));
	--local dot_result = final_direction.x*dir.x + final_direction.y*dir.z
	--local y_cross = dir.z*final_direction.x-dir.x*final_direction.y
	--_owner:set_yaw(yaw -(dot_result*_ElapsedTime*l_velocity))

	
	local x = l_desplacamiento.x*l_desplacamiento.x
	--local y = l_desplacamiento.y*l_desplacamiento.y
	local y = 0
	local z = l_desplacamiento.z*l_desplacamiento.z
	local result = math.sqrt(x+y+z)
	
	_owner:clear_cycle(_owner:get_actual_cycle_animation(),0.1);
	if result == 0 then		
		_owner:blend_cycle(1,1.0,0.1);
	else
		_owner:blend_cycle(0,1.,0.1);
	end	
end

function FnOnRenderController(_owner, _rm)

end

function FnOnDebugRender(_owner, _rm)
	local l_physXManager = CUABEngine.get_instance():get_physX_manager()
	--l_physXManager:render("player", _rm)
end