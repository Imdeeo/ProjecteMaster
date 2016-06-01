dofile("Data\\Lua\\Player\\CPlayer.lua")
dofile("Data\\Lua\\Enemies\\CEnemy.lua")
class 'CCharacterManager'
	function CCharacterManager:__init()
		self.m_Enemics={}
		self.m_Player={}
	end
		
	function CCharacterManager:LoadXML(Filename)
		--utils_log(Filename)
		utils_log("load")
		local l_XMLTreeNode=CXMLTreeNode()
		local l_Loaded=l_XMLTreeNode:load_file(Filename)
		local UABEngine = CUABEngine.get_instance()
		if l_Loaded then
			for i=0, l_XMLTreeNode:get_num_children() do
				local l_Atts=l_XMLTreeNode:get_child(i)
				local l_ElemName=l_Atts:get_name()
				if l_ElemName=="player" then
					g_Player = CPlayer(l_Atts)
					UABEngine:get_game_play_manager():add_component(g_Player)
					table.insert(self.m_Player, g_Player)
				elseif l_ElemName == "enemy" then
					--local l_Enemy = CEnemy()					
					--table.insert(self.m_Enemics, l_Enemy)
					--l_Enemy:InitEnemy(l_Atts)
				end	
			end
		else
			utils_log("File '"..Filename.."'not correctly loaded")
		end
	end
--end