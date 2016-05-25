dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")
dofile("Data\\Lua\\Triggers.lua")
dofile("Data\\Lua\\Sound\\VolumeController.lua")
dofile("Data\\Lua\\Enemies\\VisionTestEnemy\\VisionTestEnemy.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_menu = true
m_credits = false
m_options = false
g_Engine = CUABEngine.get_instance()
m_MusicSliderResult = CSliderResult(50.0, 50.0)
m_FxSliderResult = CSliderResult(50.0, 50.0)
m_Cordura = CSliderResult(50.0, 50.0)

function mainLua(level)
	InitAntweakBar()
	m_CharacterManager:LoadXML("Data\\level_"..level.."\\characters.xml")
	local l_SoundManager = CUABEngine.get_instance():get_sound_manager()
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
	g_Player.m_SoundManager:set_switch(l_switchvalue)

	g_VolumeController = VolumeController()
	g_VolumeController:SetMusicVolume(50)
	m_timerPause = 0
	m_iniciando = true
	g_TestEnemy = CVisionTestEnemy()
	g_TestEnemy:InitEnemy("automata_LOW001")
end

function luaUpdate(_ElapsedTime)
	if m_iniciando then
		m_timerPause = m_timerPause + _ElapsedTime
		if m_timerPause >= 0.2 then
			m_iniciando = false
			CUABEngine.get_instance():set_pause(true)
		end		
	end
	
	local l_Engine = CUABEngine.get_instance()
	if CInputManager.get_input_manager():is_action_active("SPEEDUP") then
		if l_Engine:get_time_scale() < 11 then
			l_Engine:set_time_scale(l_Engine:get_time_scale()+1)
		end
	end
	if CInputManager.get_input_manager():is_action_active("SPEEDDOWN") then
		if l_Engine:get_time_scale() > 1 then
			l_Engine:set_time_scale(l_Engine:get_time_scale()-1)
		end
	end
	if CInputManager.get_input_manager():is_action_active("PAUSE") then
		m_menu = true
		CUABEngine.get_instance():set_pause(true)
	end
	g_VolumeController:CheckVolumeKeys()	
end

function luaGui()
	local gui_manager = CUABEngine.get_instance():get_gui_manager()
	gui_position = CGUIPosition(580, 50, 500, 30, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
	m_Cordura = gui_manager:do_slider("Cordura", "mad_slider", gui_position,0, 100, 100, false)
			
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
			gui_manager:do_text("fontTest", "A SIMPLE SONG", coord, CGUIManager.mid_center, color)
			gui_position = CGUIPosition(500, 400, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_play = gui_manager:do_button("Play", "play_button", gui_position)
			if b_play then
				m_menu = false
				CUABEngine.get_instance():set_pause(false)
			end 
			
			gui_position = CGUIPosition(500, 460, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_options = gui_manager:do_button("Options", "options_button", gui_position)
		
			gui_position = CGUIPosition(500, 520, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_credits = gui_manager:do_button("Credits", "credits_button", gui_position)
			
			gui_position = CGUIPosition(500, 580, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_exit = gui_manager:do_button("Exit", "exit_button", gui_position)
			if b_exit then
				g_Engine:quit()
			end
		end	
	end 	 
end

