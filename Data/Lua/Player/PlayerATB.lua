function RegisterPlayerBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	local l_physXManager = UABEngine:get_physX_manager()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Player")
	
	local player = CCharacterManager.get_instance():get_resource("player")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Player\");RegisterMainBar()","")
	DebugHelper:add_lua_button("Refresh","CCharacterManager.get_instance():get_resource(\"player\"):refresh()","")
	DebugHelper:add_variable("Position x:",CDebugHelper.float,CDebugHelper.read_write,l_physXManager:get_character_controler_lua_pos_x(player.name),"")
	DebugHelper:add_variable("Position y:",CDebugHelper.float,CDebugHelper.read_write,l_physXManager:get_character_controler_lua_pos_y(player.name),"")
	DebugHelper:add_variable("Position z:",CDebugHelper.float,CDebugHelper.read_write,l_physXManager:get_character_controler_lua_pos_z(player.name),"")
	---" group='"..material_name.."' ")
	--DebugHelper:add_variable("Position y:",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group='"..material_name.."' ")
	--DebugHelper:add_variable("Position z:",CDebugHelper.float,CDebugHelper.read_write,parameter:get_value_address(),"")--" group='"..material_name.."' ")
	
	DebugHelper:register_bar()
end
