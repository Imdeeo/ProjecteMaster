function InitAntweakBar()

	local UABEngine = CUABEngine.get_instance()
	local DebugHelper = CDebugHelper.get_debug_helper()
	
	local bar = DebugHelper.SDebugBar.new
	bar.name = "cosis"
	DebugHelper:add_lua_button(bar,"cosis","cosis()","")
	DebugHelper:register_bar(bar)
	
end
function cosis()

end