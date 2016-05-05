dofile("Data\\Lua\\Player\\Player.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\Videogame\\componentsEnemy.lua")
dofile("Data\\Lua\\Antwakbar\\antweakbar.lua")


m_cinematicManager = CUABCinematicsActionManager()
m_triggerActivo1= true
m_triggerActivo2= true
m_activeA = false

function mainLua(level)
	InitAntweakBar()
	m_cinematicManager:LoadXML("Data\\level_"..level.."\\cinematic_manager.xml")
end

function luaUpdate(_ElapsedTime)
	m_cinematicManager:Update(_ElapsedTime)
	local a = false
	local b = false
	local gui_manager = CUABEngine.get_instance():get_gui_manager()
	
	local guiPosition = CGUIPosition(100,0,200,50, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
	a = gui_manager:do_button("boton1", "teula_button", guiPosition)
	local guiPosition = CGUIPosition(100,300,200,50, CGUIManager.top_left, CGUIManager.gui_absolute, CGUIManager.gui_absolute)
	b = gui_manager:do_button("boton1", "teula_button", guiPosition)

	if a then
		m_activeA = true
	end
	local color = CColor(1,1,1,1)
	local coord = Vect2f(400,50)
	gui_manager:do_text("fontTest", "tenemos texto!!", coord, CGUIManager.bottom_left, color)
	
	
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