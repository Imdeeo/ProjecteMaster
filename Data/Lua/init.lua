dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")
dofile("Data\\Lua\\Sound\\VolumeControl.lua")
dofile("Data\\Lua\\Enemy\\VisionTestEnemy.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_triggerActivo1= true
m_triggerActivo2= true
m_activeA = false


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
	l_switch.switch_name = "player_action"
	local l_switchvalue = SoundSwitchValue()
	l_switchvalue.sound_switch = l_switch
	l_switchvalue.value_name = "normal"
	g_Player.m_SoundManager:set_switch(l_switchvalue)

	local vte = VisionTestEnemy()
	vte:InitEnemy("Guardia02")
end

function luaGui()
end

function luaUpdate(_ElapsedTime)
	UpdateVolume()
	--local color = CColor(1,1,1,1)
	--local coord = Vect2f(400,50)
	--gui_manager:do_text("fontTest", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", coord, CGUIManager.bottom_left, color)
end
