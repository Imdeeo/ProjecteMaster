dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")
dofile("Data\\Lua\\Reload\\CReloadManager.lua")
dofile("Data\\Lua\\Triggers.lua")
dofile("Data\\Lua\\Sound\\VolumeController.lua")
dofile("Data\\Lua\\Enemies\\VisionTestEnemy\\VisionTestEnemy.lua")
dofile("Data\\Lua\\Commands.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
g_ReloadManager = CReloadManager()
m_menu = true
m_credits = false
m_options = false
m_fps = true
g_Engine = CUABEngine.get_instance()
m_MusicSliderResult = CSliderResult(50.0, 50.0)
m_FxSliderResult = CSliderResult(50.0, 50.0)
m_Cordura = CSliderResult(50.0, 50.0)
m_ScreenResolution = Vect2f(1280.0, 720.0)
m_ScreenFactorX = 1.0
m_ScreenFactorY = 1.0

function mainLua()
	InitAntweakBar()
	
	local l_LevelManager = g_Engine:get_level_manager()	
	local l_SoundManager = g_Engine:get_sound_manager()
	local l_WaterSoundEvent = SoundEvent()
	l_WaterSoundEvent.event_name = "water"
	local l_MainMusicEvent = SoundEvent()
	l_MainMusicEvent.event_name = "play_music"

	l_SoundManager:play_event(l_MainMusicEvent)
	l_SoundManager:play_event(l_WaterSoundEvent, "Test")

	local l_switch = SoundSwitch()
	l_switch.switch_name = "music_mode"
	local l_switchvalue = SoundSwitchValue()
	l_switchvalue.sound_switch = l_switch
	l_switchvalue.value_name = "exploration"
	--g_Player.m_SoundManager:set_switch(l_switchvalue)
	g_VolumeController = VolumeController()
	g_VolumeController:SetMusicVolume(50)
	m_timerPause = 0
	m_iniciando = true 
	
	l_LevelManager:load_level("Player")	
	
	--deactivate_gravity()
	--deactivate_player_collisions()
	--l_LevelManager:load_level("Recibidor")	
	l_LevelManager:load_level("Biblioteca")
	--l_LevelManager:load_level("Maquinas")	
	
	m_ScreenFactorX = m_ScreenResolution.x / 1920
	m_ScreenFactorY = m_ScreenResolution.y / 1080
end

function levelMainLua(level)
	level = string.gsub(level,"\/","\\")
	m_CharacterManager:LoadXML(level.."\\characters.xml")
end

function luaUpdate(_ElapsedTime)
	if not g_Engine:get_active_console() then
		if m_iniciando then
			m_timerPause = m_timerPause + _ElapsedTime
			if m_timerPause >= 0.2 then
				m_iniciando = false
				CUABEngine.get_instance():set_pause(true)
			end		
		end
	
		local l_InputManager = g_Engine:get_input_manager()
		if l_InputManager:is_action_released("DebugSpeedUp") then
			utils_log("DebugSpeedUp")
			if g_Engine:get_time_scale() < 11 then
				g_Engine:set_time_scale(g_Engine:get_time_scale()+1)
			end
		end
		if l_InputManager:is_action_released("DebugSpeedDown") then
			utils_log("DebugSpeedDown")
			if g_Engine:get_time_scale() > 1 then
				g_Engine:set_time_scale(g_Engine:get_time_scale()-1)
			end
		end
		if l_InputManager:is_action_released("DebugSanityUp") then
			--utils_log("DebugSanityUp")
			m_CharacterManager.m_Player[1]:ModifySanity(10)
		end
		if l_InputManager:is_action_released("DebugSanityDown") then
			--utils_log("DebugSanityDown")
			m_CharacterManager.m_Player[1]:ModifySanity(-10)
		end
		if l_InputManager:is_action_released("Pause") then
			m_menu = true
			CUABEngine.get_instance():set_pause(true)
		end
		if l_InputManager:is_action_released("DebugToggleFrustum") then
			g_Engine:set_frustum_active(not g_Engine:get_frustum_active())		
		end
		if l_InputManager:is_action_released("DebugConsole") then
            CUABEngine.get_instance():get_camera_controller_manager():get_main_camera():lock()
            g_Engine:set_active_console(true)    
        end    

		if l_InputManager:is_action_released("DebugToggleLoadVideo") then
			local l_videoManager = g_Engine:get_video_manager()
			l_videoManager:play_clip("bunny.ogv")
			--l_videoManager:load_clip("bunny.ogv",false)
			--l_videoManager:render_screen_clip("bunny.ogv")
		end
		--g_VolumeController:CheckVolumeKeys()	
	end
end

function luaGui()
	local gui_manager = g_Engine:get_gui_manager()
	gui_position = CGUIPosition(580, 50, 500, 30, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
	--m_Cordura = gui_manager:do_slider("Cordura", "mad_slider", gui_position,0, m_CharacterManager.m_Player[1].m_MaxSanity, m_CharacterManager.m_Player[1].m_Sanity, false)
			
	if m_fps then
		local color = CColor(1,0.2,0.2,1)
		local coord = Vect2f(1000, 10)
		local l_FPSText = string.format("%.1f fps", g_Engine:get_render_manager():get_frame_rate())
		gui_manager:do_text("fontTest", l_FPSText, coord, CGUIManager.top, color)
	end

	if m_menu then
		local color = CColor(1,1,1,1)
		local coord = Vect2f(500,100)
		local gui_position = CGUIPosition(500, 400, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
		
		if m_credits then
			coord = Vect2f(500,100)
			gui_manager:do_text("fontTest", "Joan", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,150)
			gui_manager:do_text("fontTest", "Jonathan", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,200)
			gui_manager:do_text("fontTest", "Juho", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,250)
			gui_manager:do_text("fontTest", "Julien", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,300)
			gui_manager:do_text("fontTest", "Oriol", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,350)
			gui_manager:do_text("fontTest", "Paula", coord, CGUIManager.mid_center, color)
			coord = Vect2f(500,400)
			gui_manager:do_text("fontTest", "Roberto", coord, CGUIManager.mid_center, color)
			gui_position = CGUIPosition(500, 580, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_back = gui_manager:do_button("Back", "back_button", gui_position)
			if b_back then
				m_credits = false
			end	
		elseif m_options then
			coord = Vect2f(500,150)
			gui_manager:do_text("fontTest", "Music volume", coord, CGUIManager.mid_center, color)
			gui_position = CGUIPosition(580, 200, 500, 30, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_MusicSliderResult = gui_manager:do_slider("MusicSlider", "volume_slider", gui_position,0, 100, m_MusicSliderResult.real, true)
			g_VolumeController:SetMusicVolume(m_MusicSliderResult.real)
			
			coord = Vect2f(500,350)
			gui_manager:do_text("fontTest", "Fx volume", coord, CGUIManager.mid_center, color)
			gui_position = CGUIPosition(580, 400, 500, 30, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_FxSliderResult = gui_manager:do_slider("FxSlider", "volume_slider", gui_position,0, 100, m_FxSliderResult.real, true)
			g_VolumeController:SetFXVolume(m_FxSliderResult.real)
			gui_position = CGUIPosition(500, 580, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_back = gui_manager:do_button("Back", "back_button", gui_position)
			if b_back then
				m_options = false
			end	
		else
			local l_PosX = 145 * m_ScreenFactorX
			local l_PosY = 450 * m_ScreenFactorY
			local l_Width = 330 * m_ScreenFactorX
			local l_Height = 83 * m_ScreenFactorY
			
			gui_position = CGUIPosition(l_PosX, l_PosY, l_Width, l_Height, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_play = gui_manager:do_button("Play", "play_button", gui_position)
			if b_play then
				m_menu = false
				g_Engine:set_pause(false)
			end 
			
			gui_position = CGUIPosition(l_PosX, l_PosY + l_Height, l_Width, l_Height, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_options = gui_manager:do_button("Options", "load_button", gui_position)
			
			gui_position = CGUIPosition(l_PosX, l_PosY + l_Height * 2, l_Width, l_Height, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_options = gui_manager:do_button("Options", "options_button", gui_position)
		
			gui_position = CGUIPosition(l_PosX, l_PosY + l_Height * 3, l_Width, l_Height, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_credits = gui_manager:do_button("Credits", "credits_button", gui_position)
			
			gui_position = CGUIPosition(l_PosX, l_PosY + l_Height * 4, l_Width, l_Height, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_exit = gui_manager:do_button("Exit", "exit_button", gui_position)
			if b_exit then
				g_Engine:quit()
			end
		end	
	end 	 
end

function iniciar()
	m_CharacterManager.m_Enemics[1].m_Off = false
end

function pos_player()
	utils_log("Pos x:"..g_Player.m_RenderableObject:get_position().x)
	utils_log("Pos y:"..g_Player.m_RenderableObject:get_position().y)
	utils_log("Pos z:"..g_Player.m_RenderableObject:get_position().z)
end