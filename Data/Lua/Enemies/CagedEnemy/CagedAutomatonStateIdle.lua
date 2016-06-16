function IdleFirstCagedAutomaton(args)
	utils_log("IdleFirstCagedAutomaton")
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(0,1.0,0.5)
end

function IdleUpdateCagedAutomaton(args, _ElapsedTime)
end

function IdleEndCagedAutomaton(args)
end