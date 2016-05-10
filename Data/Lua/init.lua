dofile("Data\\Lua\\Player\\PlayerMoveComponent.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")
dofile("Data\\Lua\\Characters\\CCharacterManager.lua")

m_cinematicManager = CUABCinematicsActionManager()
m_CharacterManager = CCharacterManager()
m_triggerActivo1= true
m_triggerActivo2= true
m_activeA = false

function mainLua(level)
	InitAntweakBar()
	m_CharacterManager:LoadXML("Data\\level_"..level.."\\characters.xml")
end

function luaUpdate(_ElapsedTime)
	--local color = CColor(1,1,1,1)
	--local coord = Vect2f(400,50)
	--gui_manager:do_text("fontTest", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", coord, CGUIManager.bottom_left, color)
end
