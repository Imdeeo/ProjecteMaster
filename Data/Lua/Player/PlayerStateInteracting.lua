function InteractingFirst(args)
	local l_Player = args["self"]
	l_Player.m_CameraController:lock()
end

function InteractingUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	--// Force the player face the target
	if not (l_Player.m_Target == nil) then
		ForcePlayerFaceTarget(_ElapsedTime)
	end
	
	--// Rotate player to match camera
	l_RotationXZ = Quatf()
	l_RotationY = Quatf()
	l_Rotation = l_Player.m_CameraController:get_rotation()
	l_Rotation:decouple_y(l_RotationXZ, l_RotationY)
	l_Owner:set_rotation(l_RotationY)
		
	--// Animate player
	l_Owner:clear_cycle(l_Owner:get_actual_cycle_animation(),0.1)
	if l_Displacement.y == 0 then		
		l_Owner:blend_cycle(1,1.0,0.1);
	else
		l_Owner:blend_cycle(0,1.,0.1);
	end	
end

function InteractingEnd(args)
	local l_Player = args["self"]
	l_Player.m_Target = nil
	l_Player.m_CameraController:unlock()
end
