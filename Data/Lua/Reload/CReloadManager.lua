class 'CReloadManager'
	function CReloadManager:__init()
		self.m_Resources = {}
	end

	function CReloadManager:ReloadGame(_level)
		self:removeTable(self.m_Resources)
		self:loadXML ("Data\\Lua\\Reload\\XML\\".._level..".xml")
		g_Engine:get_level_manager():get_level("Player"):get_game_play_manager():destroy()
		g_Engine:get_level_manager():get_level(_level):get_game_play_manager():destroy()
		local l_SoundManager = g_Engine:get_sound_manager()
		l_SoundManager:play_event(g_StopAllSoundsEvent)
		l_SoundManager:play_event(g_PlayMusicEvent)
		SetupLevelSounds(_level, l_SoundManager)
		
		local l_Resource = nil
		local l_Aux = nil
		local l_RemoveEnemies = true
		
		utils_log("COMENZANDO LA RECARGA!!!")
		for i=1, table.maxn(self.m_Resources) do
			l_Aux = self.m_Resources[i]
			if l_Aux[1] == "cinematic" then
				utils_log("CINEMATICAS!!!")
				l_Resource = g_Engine:get_level_manager():get_level(l_Aux[6]):get_cinematic_manager():get_resource(l_Aux[2])
				if l_Aux[3] then
					l_Resource:play()
				elseif l_Aux[4] then
					l_Resource:stop()
				elseif l_Aux[5] then
					l_Resource:pause()					
				end
			elseif l_Aux[1] == "particle" then
				utils_log("PARTICULAS!!!")
				l_Resource = g_Engine:get_level_manager():get_level(l_Aux[8]):get_layer_manager():get_layer(l_Aux[2]):get_resource(l_Aux[3])
				l_Resource:set_position(l_Aux[4])
				l_Resource:set_start(l_Aux[5])
				l_Resource:set_awake(l_Aux[6])
				l_Resource:set_visible(l_Aux[7])
				l_Resource:set_active_particles(0)
			elseif l_Aux[1] == "light" then
				utils_log("LUZ!!!")
				l_Resource = g_Engine:get_level_manager():get_level(l_Aux[5]):get_light_manager():get_resource(l_Aux[2])
				l_Resource:set_position(l_Aux[3])
				l_Resource:set_enabled(l_Aux[4])
			elseif l_Aux[1] == "trigger" then
				utils_log("TRIGGERS!!!")
				l_Resource = g_Engine:get_level_manager():get_level(l_Aux[8]):get_layer_manager():get_layer(l_Aux[2]):get_resource(l_Aux[3])
				l_Resource:set_position(l_Aux[4])
				l_Resource:set_rotation(l_Aux[5])
				l_Resource:set_visible(l_Aux[7])
				
				if l_Aux[6] then
					g_Engine:get_physX_manager():enable_trigger(l_Aux[3], "FisicasAux")
				else
					g_Engine:get_physX_manager():disable_physics(l_Aux[3], "FisicasAux")
				end
			elseif l_Aux[1] == "video" then
				g_Engine:get_video_manager():restart_clip(l_Aux[2])
			else
				utils_log("OBJETOS: "..l_Aux[3])
				l_Resource = g_Engine:get_level_manager():get_level(l_Aux[7]):get_layer_manager():get_layer(l_Aux[2]):get_resource(l_Aux[3])
				l_Resource:set_position(l_Aux[4])
				l_Resource:set_rotation(l_Aux[5])
				l_Resource:set_visible(l_Aux[6])
				l_Resource:reset_animated_values()
				
				if l_Aux[1] == "player" then
					local quat_to_turn = Quatf()
					quat_to_turn:set_from_fwd_up(l_Resource:get_rotation():get_forward_vector(), Vect3f(0,1,0))
					g_Engine:get_camera_controller_manager():choose_main_camera("MainCamera")
					g_Engine:get_camera_controller_manager():get_main_camera():set_rotation(quat_to_turn)
					self:removeTable(m_CharacterManager.m_Player)
					l_Resource:remove_animations()
				elseif l_Aux[1] == "enemy" then
					if l_RemoveEnemies then
						self:removeTable(m_CharacterManager.m_EnemicsMap[_level])
						l_RemoveEnemies = false
					end
					l_Resource:remove_animations()
				end
			end
		end
		
		levelMainLua("Data\\level_0", "Player")		
		levelMainLua("Data\\level_"..g_Engine:get_level_manager():get_level_info(_level).id, _level)
		g_Player:SetActualLevelAux(_level)
	end

	function CReloadManager:loadXML(Filename)
		local doc = XMLDocument()
		local xmlError = doc:load_file(Filename)
		local UABEngine = CUABEngine.get_instance()
		utils_log("LEYENDOOOOO")
		if xmlError == 0 then
			local l_Element = doc:first_child_element("resources"):first_child()
			while l_Element ~= nil do
				local l_Type = l_Element:get_psz_property("type", "")
				if l_Type == "player" then
					l_Player = {}
					table.insert(l_Player, l_Type)
					table.insert(l_Player, l_Element:get_psz_property("layer",""))
					table.insert(l_Player, l_Element:get_psz_property("name",""))
					table.insert(l_Player, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Player, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Player, l_Element:get_bool_property("visible", true))
					table.insert(l_Player, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Player)
				elseif l_Type == "enemy" then
					l_Enemy = {}
					table.insert(l_Enemy, l_Type)
					table.insert(l_Enemy, l_Element:get_psz_property("layer",""))
					table.insert(l_Enemy, l_Element:get_psz_property("name",""))
					table.insert(l_Enemy, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Enemy, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Enemy, l_Element:get_bool_property("visible", true))
					table.insert(l_Enemy, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Enemy)
				elseif l_Type == "object" then
					l_Object = {}
					table.insert(l_Object, l_Type)
					table.insert(l_Object, l_Element:get_psz_property("layer",""))
					table.insert(l_Object, l_Element:get_psz_property("name",""))
					table.insert(l_Object, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Object, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Object, l_Element:get_bool_property("visible", true))
					table.insert(l_Object, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Object)
				elseif l_Type == "cinematic" then
					l_Cinematic = {}
					table.insert(l_Cinematic, l_Type)
					table.insert(l_Cinematic, l_Element:get_psz_property("name",""))
					table.insert(l_Cinematic, l_Element:get_bool_property("play",false))
					table.insert(l_Cinematic, l_Element:get_bool_property("stop",false))
					table.insert(l_Cinematic, l_Element:get_bool_property("pause",false))
					table.insert(l_Cinematic, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Cinematic)
				elseif l_Type == "particle" then
					l_Particle = {}
					table.insert(l_Particle, l_Type)
					table.insert(l_Particle, l_Element:get_psz_property("layer",""))
					table.insert(l_Particle, l_Element:get_psz_property("name",""))
					table.insert(l_Particle, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Particle, l_Element:get_bool_property("start",false))
					table.insert(l_Particle, l_Element:get_bool_property("awake",false))
					table.insert(l_Particle, l_Element:get_bool_property("visible",false))
					table.insert(l_Particle, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Particle)
				elseif l_Type == "light" then
					l_Light = {}
					table.insert(l_Light, l_Type)
					table.insert(l_Light, l_Element:get_psz_property("name",""))
					table.insert(l_Light, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Light, l_Element:get_bool_property("visible", true))
					table.insert(l_Light, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Light)
				elseif l_Type == "trigger" then
					l_Trigger = {}
					table.insert(l_Trigger, l_Type)
					table.insert(l_Trigger, l_Element:get_psz_property("layer",""))
					table.insert(l_Trigger, l_Element:get_psz_property("name",""))
					table.insert(l_Trigger, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Trigger, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Trigger, l_Element:get_bool_property("enable",false))
					table.insert(l_Trigger, l_Element:get_bool_property("visible", true))
					table.insert(l_Trigger, l_Element:get_psz_property("level", ""))
					table.insert(self.m_Resources, l_Trigger)
				elseif l_Type == "video" then
					l_Video = {}
					table.insert(l_Video, l_Type)
					table.insert(l_Video, l_Element:get_psz_property("name",""))	
					table.insert(self.m_Resources, l_Video)					
				end	
				l_Element = l_Element:get_next()
			end
		else
			utils_log("File '"..Filename.."'not correctly loaded")
		end
	end

	function CReloadManager:removeTable(_table)
		for k in pairs (_table) do
			_table[k] = nil
		end
	end
--end
