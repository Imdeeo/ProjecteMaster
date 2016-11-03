function SpecialSingingStateFirst(args)
	utils_log("SpecialSingingStateFirst")
	local l_Owner = args["owner"]
	local l_Player = args["self"]
	l_Player.m_SingOnce = false
	l_Player.m_Timer = 0.0
	l_Owner:set_position(Vect3f(-0.493306, -0.000000, 4.336713))
	l_Player.m_VideoPlaying = false
	
	--g_Engine:get_level_manager():load_level("Boss",false)
	local l_LevelManager = g_Engine:get_level_manager()
	l_LevelManager:load_level("Boss",false,true,false)
end

function SpecialSingingStateUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	
	if l_Player.m_InputManager:is_action_active("Sing") and not l_Player.m_SingOnce then
		local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
		l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset 
		--l_Owner:set_position(l_NewControllerPosition)
		--l_Owner:set_rotation(Quatf(0,1,0,0))
		l_Player.m_SingOnce = true
		l_Owner:remove_action(l_Owner:get_actual_action_animation())

		l_Owner:execute_action(31, 0.1, 0.1, 1.0, true)
	end
	
	if l_Player.m_SingOnce then
		l_Player:ModifySanity(90/l_Player.m_AnimationTime)
		l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
		
			
		if l_Player.m_Timer > 9.333 then
			if l_Player.m_Timer < 13.333 then 
				local l_Value = math.min(1,(l_Player.m_Timer-9.333)/4.0)	
				local l_gui_position = CGUIPosition(0, 0, 1280, 720, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)		
				g_Engine:get_gui_manager():do_panel("fundidoNegroCantando", "fundidoNegro", l_gui_position, 0.0, l_Value)
			else
				if not l_Player.m_VideoPlaying then
					l_Player.m_VideoPlaying = true
					g_Engine:get_video_manager():load_clip("intro.ogv", false)
					--g_Engine:get_video_manager():render_screen_clip("intro.ogv")
					g_Engine:get_level_manager():get_level("Biblioteca"):set_visible(false)
				else
					g_Engine:get_level_manager():get_level("Biblioteca"):set_visible(true)
					local l_Value = 1 - math.min(1,(l_Player.m_Timer-13.333)/2)	
					local l_gui_position = CGUIPosition(0, 0, 1280, 720, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)		
					g_Engine:get_gui_manager():do_panel("fundidoNegroCantando", "fundidoNegro", l_gui_position, 0.0, l_Value)
				end
			end
		end
	end
end

function SpecialSingingStateEnd(args)
	local l_Player = args["self"]
	local l_Owner = args["owner"]
	local l_LevelManager = g_Engine:get_level_manager()
	l_LevelManager:get_level("Biblioteca"):set_has_to_update(true)
	l_Player.m_SingOnce = false
	l_Player.m_VideoPlaying = false
	local l_NewControllerPosition = l_Player.m_PhysXManager:get_character_controler_pos("player")
	l_NewControllerPosition.y = l_NewControllerPosition.y - g_StandingOffset
	l_Owner:set_position(l_NewControllerPosition)
	l_Player.m_AnimationTime = 0
	l_Player.m_CameraController:unlock()
	l_Owner:remove_action(l_Owner:get_actual_action_animation())
	l_Player:ClearCamera()
	--l_LevelManager:get_level("Player"):get_layer_manager():get_layer("solid"):get_resource("Boss"):set_visible(true)
end

function SpecialSingingStateToIdleCondition(args)
	local l_Player = args["self"]
	return l_Player.m_Timer >= l_Player.m_AnimationTime 
end
