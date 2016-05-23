dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")
dofile("Data\\Lua\\Triggers.lua")
dofile("Data\\Lua\\Sound\\VolumeController.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_menu = true
m_credits = false
g_Engine = CUABEngine.get_instance()


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
end

function luaUpdate(_ElapsedTime)
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
	g_VolumeController:CheckVolumeKeys()
end

function luaGui()
	local gui_manager = CUABEngine.get_instance():get_gui_manager()
	if m_menu then
		local color = CColor(1,1,1,1)
		local coord = Vect2f(500,100)
		gui_manager:do_text("fontTest", "A SIMPLE SONG", coord, CGUIManager.mid_center, color)
		local gui_position = CGUIPosition(500, 400, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
		
		if m_credits then
			gui_position = CGUIPosition(500, 580, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_back = gui_manager:do_button("Back", "back_button", gui_position)
			if b_back then
				m_credits = false
			end		
		else
			gui_position = CGUIPosition(500, 400, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_play = gui_manager:do_button("Play", "play_button", gui_position)
			if b_play then
				m_menu = false
				CUABEngine.get_instance():set_pause(false)
			end 
			
			gui_position = CGUIPosition(500, 460, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_options = gui_manager:do_button("Options", "options_button", gui_position)
			if b_options then
				
			end 
			
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

function mipene(a,b)
	utils_log("pene: "..a.."\npene2: "..b)
end
