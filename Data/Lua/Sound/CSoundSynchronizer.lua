class 'CSoundSynchronizer' 
	function CSoundSynchronizer:__init(actor, owner, _SoundEvent)
		self.m_Offset = 0
		self.m_RepeatsPerEvent = 1 -- this has to be same as in Wwise
		self.m_RepeatsPerCycle = 1 -- for example two repeats of steps for one cycle of walking
		-- if SetAnimationDuration() is called at creation time, the first sync can be done right away
		-- otherwise it is set automatically after m_AdjustmentTimer goes off (and the animation transition is over)
		self.m_AdjustmentTimer = 0.5 -- delay for acquiring animation time for the new animation

		self.m_SoundEvent = _SoundEvent
		self.m_Actor = actor
		self.m_Owner = owner
		self.m_ReadyToSync = false
		self.m_DurationChecked = false
		self.m_TimeToNextScheduling = 0
	end

	function CSoundSynchronizer:SetAnimationDuration(_Duration)
		self.m_RepeatInterval = _Duration / self.m_RepeatsPerCycle
		self.m_SyncInterval = self.m_RepeatInterval * self.m_RepeatsPerEvent
		self.m_ReadyToSync = true
	end

	function CSoundSynchronizer:Sync(_ElapsedTime)
		if not self.m_DurationChecked then
			self.m_AdjustmentTimer = self.m_AdjustmentTimer - _ElapsedTime
			if self.m_AdjustmentTimer < 0 then
				utils_log("SoundSynchronizer adjusted")
				self.m_DurationChecked = true
				self:SetAnimationDuration(self.m_Owner:get_animation_duration())
			end
		end

		if not self.m_ReadyToSync then
			return
		end

		self.m_TimeToNextScheduling = self.m_TimeToNextScheduling - _ElapsedTime
		if self.m_TimeToNextScheduling <= 0 then
			local l_AnimationTime = self.m_Owner:get_animation_time()
			local l_Delay = (self.m_Offset - l_AnimationTime) % self.m_RepeatInterval
			self.m_TimeToNextScheduling = l_Delay + self.m_SyncInterval - self.m_RepeatInterval + 0.2*self.m_RepeatInterval

			local l_SoundManager = self.m_Actor.m_SoundManager
			l_SoundManager:set_rtpc_value(g_InitialDelayRTPC, l_Delay, self.m_Owner)
			l_SoundManager:play_event(self.m_SoundEvent, self.m_Owner)
		end
	end
