function FinishFirst(args)
	utils_log("FinishFirst")
	local l_Player = args["self"]
	l_Player.m_Timer = 0.0
end

function FinishUpdate(args, _ElapsedTime)
	local l_Player = args["self"]
	local l_Owner = args["owner"]	

	l_Player.m_Timer = l_Player.m_Timer + _ElapsedTime
	if l_Player.m_Timer > 5.0 then
		if l_Player.m_Timer < 10.0 then 
			local l_Value = math.min(1,(l_Player.m_Timer-5.0)/5.0)
			local l_gui_position = CGUIPosition(0, 0, m_ScreenResolution.x, m_ScreenResolution.y, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)		
			g_Engine:get_gui_manager():do_panel("fundidoNegroCantando", "fundidoNegro", l_gui_position, 0.0, l_Value)
		else
			if not l_Player.m_VideoPlaying then
				l_Player.m_VideoPlaying = true
				g_Engine:get_video_manager():load_clip("credits.ogv", false)
				g_Engine:get_video_manager():render_screen_clip("credits.ogv")
				g_SoundManager:play_event(g_CreditsVideoEvent)
			end
		end
	end
end

function FinishEnd(args)
	local l_Player = args["self"]
end