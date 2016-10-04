class 'CReloadManager'
	function CReloadManager:__init()
		self.m_Resources = {}
	end

	function CReloadManager:ReloadGame(_level)
		self:removeTable(self.m_Resources)
		self:loadXML ("Data\\Lua\\Reload\\XML\\level_".._level..".xml")

		g_Engine:get_game_play_manager():destroy()
		
		local l_Resource = nil
		local l_Aux = nil
		local l_RemoveEnemies = true
		
		for i=1, table.maxn(self.m_Resources) do
			l_Aux = self.m_Resources[i]
			if l_Aux[1] ~= "cinematic" then
				l_Resource = g_Engine:get_layer_manager():get_layer(l_Aux[2]):get_resource(l_Aux[3])
				l_Resource:set_position(l_Aux[4])
				l_Resource:set_rotation(l_Aux[5])
				l_Resource:set_visible(l_Aux[6])
				l_Resource:reset_animated_values()
				
				if l_Aux[1] == "player" then
					local quat_to_turn = Quatf()
					quat_to_turn:set_from_fwd_up(l_Resource:get_rotation():get_forward_vector(), Vect3f(0,1,0))
					g_Engine:get_camera_controller_manager():get_main_camera():set_rotation(quat_to_turn)
					self:removeTable(m_CharacterManager.m_Player)
					l_Resource:remove_animations()
				elseif l_Aux[1] == "enemy" then
					if l_RemoveEnemies then
						self:removeTable(m_CharacterManager.m_Enemics)
						l_RemoveEnemies = false
					end
					l_Resource:remove_animations()
				end
			else
				g_Engine:get_cinematic_manager():get_resource(l_Aux[3]):stop()
			end
		end
		
		levelMainLua("Data\\level_0")		
		levelMainLua("Data\\level_".._level)
	end

	function CReloadManager:loadXML(Filename)
		local doc = XMLDocument()
		local xmlError = doc:load_file(Filename)
		local UABEngine = CUABEngine.get_instance()
		
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
					table.insert(self.m_Resources, l_Player)
				elseif l_Type == "enemy" then
					l_Enemy = {}
					table.insert(l_Enemy, l_Type)
					table.insert(l_Enemy, l_Element:get_psz_property("layer",""))
					table.insert(l_Enemy, l_Element:get_psz_property("name",""))
					table.insert(l_Enemy, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Enemy, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Enemy, l_Element:get_bool_property("visible", true))
					table.insert(self.m_Resources, l_Enemy)
				elseif l_Type == "object" then
					l_Object = {}
					table.insert(l_Object, l_Type)
					table.insert(l_Object, l_Element:get_psz_property("layer",""))
					table.insert(l_Object, l_Element:get_psz_property("name",""))
					table.insert(l_Object, l_Element:get_vect3f_property("position",Vect3f(0,0,0)))
					table.insert(l_Object, l_Element:get_quat_property("rotation",Quatf(0,0,0,1)))
					table.insert(l_Object, l_Element:get_bool_property("visible", true))
					table.insert(self.m_Resources, l_Object)
				elseif l_Type == "cinematic" then
					l_Cinematic = {}
					table.insert(l_Cinematic, l_Type)
					table.insert(l_Cinematic, l_Element:get_psz_property("layer",""))
					table.insert(l_Cinematic, l_Element:get_psz_property("name",""))
					table.insert(self.m_Resources, l_Cinematic)
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