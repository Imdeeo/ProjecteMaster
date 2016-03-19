dofile("Data\\Lua\\Player\\Player.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")


m_cinematicManager = CUABCinematicsActionManager()

function mainLua(level)
	InitAntweakBar()
	m_cinematicManager:LoadXML("Data\\level_"..level.."\\cinematic_manager.xml")
end

function luaUpdate(_ElapsedTime)
	m_cinematicManager:Update(_ElapsedTime)
end


function onTriggerActivarMolinoBox(actor)
	if actor == "player" then
		--activarMolino()
	end
end

function onTriggerActivarMolinoSphere(actor)
	if actor == "player" then
		--activarMolino()
	end
end

function activarMolino()
	if m_cinematicManager.m_Play then
		utils_log("true")
	else
		utils_log("false")
	end
	m_cinematicManager.m_Play = true
end