function RegisterPlayerBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	DebugHelper:remove_bar("MainBar")
	DebugHelper:start_register_bar("Player")
	
	DebugHelper:add_lua_button("Back","CDebugHelper.get_debug_helper():remove_bar(\"Player\");RegisterMainBar()","");
	DebugHelper:add_lua_button("Refresh","CCharacterManager.get_instance():get_resource(\"player\"):refresh()","");
	
	DebugHelper:register_bar()
end
