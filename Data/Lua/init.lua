dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_menu = true


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
	if m_menu then
		local color = CColor(1,1,1,1)
		local coord = Vect2f(500,100)
		local gui_manager = CUABEngine.get_instance():get_gui_manager()
		gui_manager:do_text("fontTest", "A SIMPLE SONG", coord, CGUIManager.mid_center, color)
		local gui_position = CGUIPosition(500, 400, 50, 30, CGUIManager.mid_center, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
		
		local a = gui_manager:do_button("Play", "teula_button", gui_position)
		if a then
			m_menu = false
			CUABEngine.get_instance():set_pause(false)
		end 
	else
	
	end 	 
end

function mipene(a,b)
	utils_log("pene: "..a.."\npene2: "..b)
end
