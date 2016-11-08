class 'CTimerManager'
	function CTimerManager:__init()
		--utils_log("Timer manager created.")
		self.m_Timers = {}

		-- for debugging
		self.m_Counter = 0 
	end

	function CTimerManager:ExecuteLater(_Delay, _Function)
		table.insert(self.m_Timers, {_Delay, _Function})
	end

	function CTimerManager:Update(_ElapsedTime)
		for i, timer in ipairs(self.m_Timers) do
			timer[1] = timer[1] - _ElapsedTime
			if timer[1] <= 0 then
				--utils_log("Executing...")
				table.remove(self.m_Timers, i)
				timer[2]()
			end
		end
	end

	function CTimerManager:Clear()
		self.m_Timers = {}
	end
--end

