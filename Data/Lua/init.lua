dofile("Data\\Lua\\Player\\Player.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionManager.lua")
dofile("Data\\Lua\\componentsEnemy.lua")
dofile("Data\\Lua\\antweakbar.lua")
function mainLua()
	InitPlayer()
	InitAntweakBar()
	InitEnemyMove()
end

function luaUpdate(_ElapsedTime)

end