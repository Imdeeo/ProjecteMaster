State = {}
State.__index = State

function State.create(update_function)
	local stt = {}
	setmetatable(stt,State)
	stt.update_function = update_function
	stt.n_conditions = 0
	stt.conditions = {}
	return stt
end

function State:add_condition(condition,state2go)
	self.conditions[self.n_conditions] = {}
	self.conditions[self.n_conditions].condition = condition
	self.conditions[self.n_conditions].state2go = state2go
	self.n_conditions = self.n_conditions + 1
end

StateMachine = {}
StateMachine.__index = StateMachine

function StateMachine.create()
	local st_mch = {}
	setmetatable(st_mch,StateMachine)
	st_mch.states = {}
	st_mch.n_states = 0
	st_mch.actual_state = -1
	return st_mch
end

function StateMachine:add_state(state)
	self.states[self.n_states] = state
	self.n_states = self.n_states + 1
end

function StateMachine:start(state_to_start)
	self.actual_state = state_to_start
end
function StateMachine:start()
	self:start(0)
end

function StateMachine:update(elapsed_time)
	local state = self.states[self.actual_state]
	state.update_function(elapsed_time)
	for cond in state.conditions do
		if cond.condition() then
			self.actual_state = cond.state2go
		end
	end
end