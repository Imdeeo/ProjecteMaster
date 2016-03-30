dofile("Data\\Lua\\Player\\Player.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")


m_cinematicManager = CUABCinematicsActionManager()
m_triggerActivo1= true
m_triggerActivo2= true
function mainLua(level)
	InitAntweakBar()
	m_cinematicManager:LoadXML("Data\\level_"..level.."\\cinematic_manager.xml")
end

function luaUpdate(_ElapsedTime)
	m_cinematicManager:Update(_ElapsedTime)
	local a = false
	local gui_manager = CUABEngine.get_instance():get_gui_manager()
	utils_log("debug")
	a = gui_manager:do_button("boton1", "teula_button", {10,10,50,50})
	utils_log(""..a)
end


function onTriggerActivarMolinoBox(actor)
	if actor == "player" then
		if m_triggerActivo1 then
			m_triggerActivo1 = false
			activarMolino()
		end
	end
end

function onTriggerActivarMolinoSphere(actor)
	if actor == "player" then
		if m_triggerActivo2 then
			m_triggerActivo2 = false
			activarMolino()
		end
	end
end

function activarMolino()
	m_cinematicManager.m_Play = true
end