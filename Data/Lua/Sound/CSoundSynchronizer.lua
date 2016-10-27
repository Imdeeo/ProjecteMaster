class 'CSoundSynchronizer' 
	function CSoundSynchronizer:__init(actor, owner, cyclesBetweenSyncs)
		self.m_Actor = actor
		self.m_Owner = owner
		self.m_CyclesBetweenSyncs = cyclesBetweenSyncs
		self.m_EventsPerCycle = 1
		self.m_StartSequenceEvent = nil
		self.m_DelayRTPC = SoundRTPC()
		self.m_DelayRTPC.rtpc_name = "initial_delay"
		self.m_FirstSync = true
		self.m_Offset = 0
		self.m_ActivationTimer = 0.5
	end

	function CSoundSynchronizer:Sync(elapsedTime)
		if self.m_FirstSync then
			self.m_ActivationTimer = self.m_ActivationTimer - elapsedTime
			if self.m_ActivationTimer < 0 then
				self.m_FirstSync = false
				local l_AnimationDuration = self.m_Owner:get_animation_duration()
				self.m_SyncInterval = l_AnimationDuration * self.m_CyclesBetweenSyncs
				self.m_EventInterval = l_AnimationDuration / self.m_EventsPerCycle
				self.m_TimeToNextScheduling = 0
			else
				return
			end
		end

		self.m_TimeToNextScheduling = self.m_TimeToNextScheduling - elapsedTime
		if self.m_TimeToNextScheduling <= 0 then
			local l_AnimationTime = self.m_Owner:get_animation_time()
			local l_Delay = (self.m_Offset - l_AnimationTime) % self.m_EventInterval
			self.m_TimeToNextScheduling = l_Delay + self.m_SyncInterval - self.m_EventInterval

			local l_SoundManager = self.m_Actor.m_SoundManager
			l_SoundManager:set_rtpc_value(self.m_DelayRTPC, l_Delay, self.m_Owner)
			l_SoundManager:play_event(self.m_StartSequenceEvent, self.m_Owner)
			
			utils_log("Scheduling next sound after "..l_Delay.." seconds. Anim. dur.: "..self.m_Owner:get_animation_duration())
		end
	end
