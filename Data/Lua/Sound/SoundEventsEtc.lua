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
g_PlayerRunSoundEvent = SoundEvent()
g_PlayerRunSoundEvent.event_name = "Run_Player"
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
g_ClimbStartSoundEvent = SoundEvent()
g_ClimbStartSoundEvent.event_name = "Climb_Start"
g_ClimbLoopSoundEvent = SoundEvent()
g_ClimbLoopSoundEvent.event_name = "Climb_Loop"
g_ClimbEndSoundEvent = SoundEvent()
g_ClimbEndSoundEvent.event_name = "Climb_End"

-- Interaction
g_InteractionSoundEvents = {}
-- Recibidor
g_OpenDrawerSoundEvent = SoundEvent()
g_OpenDrawerSoundEvent.event_name = "Drawer_Open"
g_CloseDrawerSoundEvent = SoundEvent()
g_CloseDrawerSoundEvent.event_name = "Drawer_Close"
g_ForceDrawerSoundEvent = SoundEvent()
g_ForceDrawerSoundEvent.event_name = "Drawer_Force"
-- Biblioteca
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
g_EnterLibrarySondEvent = SoundEvent()
g_EnterLibrarySondEvent.event_name = "Enter_Library"
g_OpenSecretDoorSoundEvent = SoundEvent()
g_OpenSecretDoorSoundEvent.event_name = "Open_Secret_Door"
g_DoorSlam1Event = SoundEvent()
g_DoorSlam1Event.event_name = "Door_Slam_1"
g_DoorSlam2Event = SoundEvent()
g_DoorSlam2Event.event_name = "Door_Slam_2"
-- Maquinas
g_PickValveEvent = SoundEvent()
g_PickValveEvent.event_name = "Pick_Valve"
g_InputValveEvent = SoundEvent()
g_InputValveEvent.event_name = "Input_Valve"
g_OpenValveDoor = SoundEvent()
g_OpenValveDoor.event_name = "Open_Valve_Door"
-- Pasillo
g_JailEvent = SoundEvent()
g_JailEvent.event_name = "Jail"
g_BoilerRoomSoundEvent = SoundEvent()
g_BoilerRoomSoundEvent.event_name = "Boiler_Room"
g_PendulumSoundEvent = SoundEvent()
g_PendulumSoundEvent.event_name = "Pendulum"

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
g_ConveyorBeltSoundEvent = SoundEvent()
g_ConveyorBeltSoundEvent.event_name = "Conveyor_Belt"
g_HourglassSoundEvent = SoundEvent()
g_HourglassSoundEvent.event_name = "Hourglass_Start"
g_BeltSoundEvent = SoundEvent()
g_BeltSoundEvent.event_name = "Belt_Start"
g_CrusherSoundEvent = SoundEvent()
g_CrusherSoundEvent.event_name = "Crusher_Start"
g_MiniwheelSoundEvent = SoundEvent()
g_MiniwheelSoundEvent.event_name = "Miniwheel_Start"
g_CauldronSoundEvent = SoundEvent()
g_CauldronSoundEvent.event_name = "Cauldron_Start"
g_NewcomenSoundEvent = SoundEvent()
g_NewcomenSoundEvent.event_name = "Newcomen_Start"
g_TeslaSoundEvent = SoundEvent()
g_TeslaSoundEvent.event_name = "Tesla_Start"
g_CylinderSoundEvent = SoundEvent()
g_CylinderSoundEvent.event_name = "Cylinder_Start"
g_SteamSoundEvent = SoundEvent()
g_SteamSoundEvent.event_name = "Steam_Start"
g_JailSoundEvent = SoundEvent()
g_JailSoundEvent.event_name = "Jail_Start"
g_CaveSoundEvent = SoundEvent()
g_CaveSoundEvent.event_name = "Cave_Start"
g_ButhcerSoundEvent = SoundEvent()
g_ButhcerSoundEvent.event_name = "Butcher_Start"

-- Enemies
g_AutomatonActivateSoundEvent = SoundEvent()
g_AutomatonActivateSoundEvent.event_name = "Automaton_Activate"

g_AutomatonStepSoundEvent = SoundEvent()
g_AutomatonStepSoundEvent.event_name = "Automaton_Step"
g_AutomatonStartPatrolEvent = SoundEvent()
g_AutomatonStartPatrolEvent.event_name = "Automaton_Start_Patrol"
g_AutomatonStopPatrolEvent = SoundEvent()
g_AutomatonStopPatrolEvent.event_name = "Automaton_Stop_Patrol"
g_BossStepSoundEvent = SoundEvent()
g_BossStepSoundEvent.event_name = "Boss_Step"

-- Jaheem
g_DondeNosLlevanVoiceEvent = SoundEvent()
g_DondeNosLlevanVoiceEvent.event_name = "Jaheem_Donde_Nos_Llevan"
g_MeLargoVoiceEvent = SoundEvent()
g_MeLargoVoiceEvent.event_name = "Jaheem_Me_Largo"
g_NoNoNoVoiceEvent = SoundEvent()
g_NoNoNoVoiceEvent.event_name = "Jaheem_No_No_No"
g_PorFavorNoVoiceEvent = SoundEvent()
g_PorFavorNoVoiceEvent.event_name = "Jaheem_Por_Favor_No"
g_PorQueHacesEstoVoiceEvent = SoundEvent()
g_PorQueHacesEstoVoiceEvent.event_name = "Jaheem_Por_Que_Haces_Esto"
g_QueDemoniosVoiceEvent = SoundEvent()
g_QueDemoniosVoiceEvent.event_name = "Jaheem_Que_Demonios"
g_QueEraEsoVoiceEvent = SoundEvent()
g_QueEraEsoVoiceEvent.event_name = "Jaheem_Que_Era_Eso"
g_ReaccionAlHijoVoiceEvent = SoundEvent()
g_ReaccionAlHijoVoiceEvent.event_name = "Jaheem_Reaccion_Al_Hijo"
g_RespiracionesVoiceEvent = SoundEvent()
g_RespiracionesVoiceEvent.event_name = "Jaheem_Respiraciones"
g_RisaVoiceEvent = SoundEvent()
g_RisaVoiceEvent.event_name = "Jaheem_Risa"
g_TengoQueSalirVoiceEvent = SoundEvent()
g_TengoQueSalirVoiceEvent.event_name = "Jaheem_Tengo_Que_Salir"
g_GritoVoiceEvent = SoundEvent()
g_GritoVoiceEvent.event_name = "Jaheem_Grito"


-- Ysmael
g_MuertePorCorduraVoiceEvent = SoundEvent()
g_MuertePorCorduraVoiceEvent.event_name = "Muerte_Cordura"
g_MuerteViolentaVoiceEvent = SoundEvent()
g_MuerteViolentaVoiceEvent.event_name = "Muerte_Violenta"
g_MonologoVoiceEvent = SoundEvent()
g_MonologoVoiceEvent.event_name = "Ysmael_Monologo_Inicial"
g_HijoVoiceEvent = SoundEvent()
g_HijoVoiceEvent.event_name = "Ysmael_Hijo"

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
