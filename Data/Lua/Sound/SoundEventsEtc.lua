-- General
g_StopAllSoundsEvent = SoundEvent()
g_StopAllSoundsEvent.event_name = "Stop_All"
g_BreakAllSoundsEvent = SoundEvent()
g_BreakAllSoundsEvent.event_name = "Break_All"
g_PauseAllSoundsEvent = SoundEvent()
g_PauseAllSoundsEvent.event_name = "Pause_All"
g_ResumeAllSoundsEvent = SoundEvent()
g_ResumeAllSoundsEvent.event_name = "Resume_All"

g_InitialDelayRTPC = SoundRTPC()
g_InitialDelayRTPC.rtpc_name = "initial_delay"
g_SecondaryDelayRTPC = SoundRTPC()
g_SecondaryDelayRTPC.rtpc_name = "secondary_delay"
g_PitchRTPC = SoundRTPC()
g_PitchRTPC.rtpc_name = "pitch_difference"

-- Level
g_LevelStateGroup = SoundState()
g_LevelStateGroup.state_name = "Level"
g_RecibidorState = SoundStateValue()
g_RecibidorState.sound_state = g_LevelStateGroup
g_RecibidorState.value_name = "Recibidor"
g_BibliotecaState = SoundStateValue()
g_BibliotecaState.sound_state = g_LevelStateGroup
g_BibliotecaState.value_name = "Biblioteca"
g_MaquinasState = SoundStateValue()
g_MaquinasState.sound_state = g_LevelStateGroup
g_MaquinasState.value_name = "SalaMaquinas"
g_PasilloState = SoundStateValue()
g_PasilloState.sound_state = g_LevelStateGroup
g_PasilloState.value_name = "Pasillo"
g_BossState = SoundStateValue()
g_BossState.sound_state = g_LevelStateGroup
g_BossState.value_name = "SalaBoss"

g_LevelStates = {}
g_LevelStates["Recibidor"] = g_RecibidorState
g_LevelStates["Biblioteca"] = g_BibliotecaState
g_LevelStates["Maquinas"] = g_MaquinasState
g_LevelStates["Pasillo"] = g_PasilloState
g_LevelStates["Boss"] = g_BossState


-- Music
g_PlayMusicEvent = SoundEvent()
g_PlayMusicEvent.event_name = "Play_Music"

-- Player
g_PlayerJumpSoundEvent = SoundEvent()
g_PlayerJumpSoundEvent.event_name = "Jump_Player"
g_PlayerLandSoundEvent = SoundEvent()
g_PlayerLandSoundEvent.event_name = "Land_Player"
g_PlayerStepSoundEvent = SoundEvent()
g_PlayerStepSoundEvent.event_name = "Step_Player"
g_SingSoundEvent = SoundEvent()
g_SingSoundEvent.event_name = "Song_Start"
g_StopSingingSoundEvent = SoundEvent()
g_StopSingingSoundEvent.event_name = "Song_Stop"
g_IsolationRTPC = SoundRTPC()
g_IsolationRTPC.rtpc_name = "player_isolation"
g_ActionStateGroup = SoundState()
g_ActionStateGroup.state_name = "Action"
g_SingingActionState = SoundStateValue()
g_SingingActionState.sound_state = g_ActionStateGroup
g_SingingActionState.value_name = "Singing"
g_ExplorationActionState = SoundStateValue()
g_ExplorationActionState.sound_state = g_ActionStateGroup
g_ExplorationActionState.value_name = "Exploraton"

-- Interaction
g_InteractionSoundEvents = {}
g_OpenDrawerSoundEvent = SoundEvent()
g_OpenDrawerSoundEvent.event_name = "Drawer_Open"
g_CloseDrawerSoundEvent = SoundEvent()
g_CloseDrawerSoundEvent.event_name = "Drawer_Close"
g_ForceDrawerSoundEvent = SoundEvent()
g_ForceDrawerSoundEvent.event_name = "Drawer_Force"
g_BookSwitchSoundEvent = SoundEvent()
g_BookSwitchSoundEvent.event_name = "Book_Switch"
g_ProjectorSoundEvent = SoundEvent()
g_ProjectorSoundEvent.event_name = "Projector_Start"
g_ClueSheetPickupSoundEvent = SoundEvent()
g_ClueSheetPickupSoundEvent.event_name = "Paper_Sheet_Pick"
g_ArtifactPickupSoundEvent = SoundEvent()
g_ArtifactPickupSoundEvent.event_name = "Artifact_Pick"
g_ArtifactUseSoundEvent = SoundEvent()
g_ArtifactUseSoundEvent.event_name = "Artifact_Use"

-- Organ
g_OrganNoteSwitch = SoundSwitch()
g_OrganNoteSwitch.switch_name = "Organ_Note"

g_OrganNoteC = SoundSwitchValue()
g_OrganNoteC.sound_switch = g_OrganNoteSwitch
g_OrganNoteC.value_name = "Organ_C"

g_OrganNoteD = SoundSwitchValue()
g_OrganNoteD.sound_switch = g_OrganNoteSwitch
g_OrganNoteD.value_name = "Organ_D"

g_OrganNoteE = SoundSwitchValue()
g_OrganNoteE.sound_switch = g_OrganNoteSwitch
g_OrganNoteE.value_name = "Organ_E"

g_OrganNoteF = SoundSwitchValue()
g_OrganNoteF.sound_switch = g_OrganNoteSwitch
g_OrganNoteF.value_name = "Organ_F"

g_OrganNoteG = SoundSwitchValue()
g_OrganNoteG.sound_switch = g_OrganNoteSwitch
g_OrganNoteG.value_name = "Organ_G"

g_OrganNoteA = SoundSwitchValue()
g_OrganNoteA.sound_switch = g_OrganNoteSwitch
g_OrganNoteA.value_name = "Organ_A"

g_OrganNoteB = SoundSwitchValue()
g_OrganNoteB.sound_switch = g_OrganNoteSwitch
g_OrganNoteB.value_name = "Organ_B"

g_OrganPlayNoteEvent = SoundEvent()
g_OrganPlayNoteEvent.event_name = "Organ_Play_Note"

-- Environment
g_BookShelfSoundEvent = SoundEvent()
g_BookShelfSoundEvent.event_name = "Bookshelf_move"
g_FireSoundEvent = SoundEvent()
g_FireSoundEvent.event_name = "Fire_Start"
g_CogwheelsSoundEvent = SoundEvent()
g_CogwheelsSoundEvent.event_name = "Cogwheels_Start"

-- Enemies
g_AutomatonActivateSoundEvent = SoundEvent()
g_AutomatonActivateSoundEvent.event_name = "Automaton_Activate"

g_AutomatonStepSoundEvent = SoundEvent()
g_AutomatonStepSoundEvent.event_name = "Automaton_Step"
g_AutomatonStartPatrolEvent = SoundEvent()
g_AutomatonStartPatrolEvent.event_name = "Automaton_Start_Patrol"
g_AutomatonStopPatrolEvent = SoundEvent()
g_AutomatonStopPatrolEvent.event_name = "Automaton_Stop_Patrol"

--[[
g_AutomatonStartBeatingSoundEvent = SoundEvent()
self.m_BeatSoundEvent.event_name = "Automaton_Start_Beating"
self.m_StopBeatSoundEvent = SoundEvent()
self.m_StopBeatSoundEvent.event_name = "Automaton_Stop_Beating"
]]
g_AutomatonBeatOnceEvent = SoundEvent()
g_AutomatonBeatOnceEvent.event_name = "Automaton_Beat_1"
g_AutomatonBeatSequence6Event = SoundEvent()
g_AutomatonBeatSequence6Event.event_name = "Automaton_Beat_6"
g_AutomatonStopBeatingSoundEvent = SoundEvent()
g_AutomatonStopBeatingSoundEvent.event_name = "Automaton_Stop_Beating"
