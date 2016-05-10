State = {}
State.__index = State

function State.create(update_function)
	local stt = {}
	setmetatable(stt,State)
	stt.update_function = update_function
	stt.do_first_function = function() utils_log("first"); end
	stt.do_end_function = function()utils_log("end");end
	stt.n_conditions = 0
	stt.conditions = {}
	stt.is_first = false
	return stt
end

function State:add_condition(condition,state2go)
	self.conditions[self.n_conditions] = {}
	local cond = {}
	cond.condition = condition
	cond.state2go = state2go
	self.conditions[self.n_conditions] = cond
	self.n_conditions = self.n_conditions + 1
end

function State:set_do_first_function(_do_first_function)
	self.do_first_function = _do_first_function
end

function State:set_do_end_function(_do_end_function)
	self.do_end_function = _do_end_function
end

StateMachine = {}
StateMachine.__index = StateMachine

function StateMachine.create()
	local st_mch = {}
	setmetatable(st_mch,StateMachine)
	st_mch.states = {}
	st_mch.states_vect = {}
	st_mch.n_states = 0
	st_mch.actual_state = -1
	return st_mch
end

function StateMachine:add_state(name,state)
	self.states[name] = state
	self.states_vect[self.n_states] = name
	self.n_states = self.n_states + 1
end

function StateMachine:start()
	self:start__s(self.states_vect[0])
end
function StateMachine:start__s(state_to_start)
	self.actual_state = state_to_start
	self.states[self.actual_state].is_first = true
	utils_log("state: "..self.actual_state)
end

function StateMachine:update(args,elapsed_time)
	
	local state = self.states[self.actual_state]
	if(state.is_first) then
		state.do_first_function(args)
		self.states[self.actual_state].is_first = false
	end
	
	state.update_function(args,elapsed_time)

	local change = false
	local prev_state
	for i = 0, state.n_conditions - 1 do
		local cond = state.conditions[i]
		if cond.condition() then
			change = true
			prev_state = self.actual_state
			self.actual_state = cond.state2go
			self.states[self.actual_state].is_first = true
		end
	end
	if(change)then
		self.states[prev_state].do_end_function(args)
	end
end