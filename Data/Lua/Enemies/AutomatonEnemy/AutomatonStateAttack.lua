function AttackFirstAutomaton(args)
	utils_log("AttackFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(0,0.1)
end

function AttackUpdateAutomaton(args, _ElapsedTime)
	
end

function AttackEndAutomaton(args)
	utils_log("AttackEnd")
end