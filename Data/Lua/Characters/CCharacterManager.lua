dofile("Data\\Lua\\Player\\CPlayer.lua")
class 'CCharacterManager'
	function CCharacterManager:__init()
		self.m_Enemics={}
		self.m_Player={}
	end
		
	function CCharacterManager:LoadXML(Filename)
		--utils_log(Filename)
		local l_XMLTreeNode=CXMLTreeNode()
		local l_Loaded=l_XMLTreeNode:load_file(Filename)
		if l_Loaded then
			for i=0, l_XMLTreeNode:get_num_children() do
				local l_Atts=l_XMLTreeNode:get_child(i)
				local l_ElemName=l_Atts:get_name()
				if l_ElemName=="player" then
					g_Player = CPlayer()
					g_Player:InitPlayer(l_Atts)
					table.insert(self.m_Player, g_Player)
				elseif l_ElemName == "enemy" then
					local l_Enemy = CEnemy()
					l_Enemy:InitEnemy(l_Atts)
					table.insert(self.m_Enemics, l_Enemy)
				end	
			end
		else
			utils_log("File '"..Filename.."'not correctly loaded")
		end
	end
--end