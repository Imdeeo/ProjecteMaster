dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_menu = true
m_credits = false


function mainLua(level)
	InitAntweakBar()
	m_CharacterManager:LoadXML("Data\\level_"..level.."\\characters.xml")
	--local l_SoundEvent = SoundEvent()
	--l_SoundEvent.event_name = "water"
	--CUABEngine.get_instance():get_sound_manager():play_event(l_SoundEvent, "Test")
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
			local b_back = gui_manager:do_button("Back", "teula_button", gui_position)
			if b_back then
				m_credits = false
			end		
		else
			gui_position = CGUIPosition(500, 400, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_play = gui_manager:do_button("Play", "teula_button", gui_position)
			if b_play then
				m_menu = false
				CUABEngine.get_instance():set_pause(false)
			end 
			
			gui_position = CGUIPosition(500, 460, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_options = gui_manager:do_button("Options", "teula_button", gui_position)
			if b_options then
				
			end 
			
			gui_position = CGUIPosition(500, 520, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			m_credits = gui_manager:do_button("Credits", "teula_button", gui_position)
			
			gui_position = CGUIPosition(500, 580, 100, 50, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
			local b_exit = gui_manager:do_button("Exit", "teula_button", gui_position)
		end	
		
		
		if b_exit then
			
		end 
	end 	 
end

function mipene(a,b)
	utils_log("pene: "..a.."\npene2: "..b)
end
