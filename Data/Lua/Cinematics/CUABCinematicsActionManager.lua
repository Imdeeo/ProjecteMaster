dofile("Data\\Lua\\Cinematics\\CUABCinematicsAction.lua")
dofile("Data\\Lua\\Cinematics\\CUABCinematicsActionSetCamera.lua")

class 'CUABCinematicsActionManager'
	function CUABCinematicsActionManager:__init()
		self.m_Actions={}
		self.m_MaxTime=0
		self.m_CurrentTime=0.0
		self.m_CurrentAction=1
		self.m_CurrentExecutedAction=1
		self.m_Play = false
	end

	function CUABCinematicsActionManager:Update(ElapsedTime)
		if self.m_Play then
			self.m_CurrentTime=self.m_CurrentTime+ElapsedTime
			if self.m_CurrentTime >= self.m_MaxTime then
				self.m_CurrentTime = 0
				self.m_Play = false
				
				for i=1, (#self.m_Actions) do
					if self.m_Actions[i]:IsActive() or self.m_Actions[i]:MustUpdate() then
						self.m_Actions[i]:Update(ElapsedTime)
					end
				end
			else
				
				local l_CurrentActionId=self.m_CurrentAction
				if l_CurrentActionId>#(self.m_Actions) then
					return
				end

				while self.m_CurrentExecutedAction<=(#self.m_Actions) and self.m_Actions[self.m_CurrentExecutedAction]:GetTime()<=self.m_CurrentTime do
					self.m_Actions[self.m_CurrentExecutedAction]:Execute()
					self.m_CurrentExecutedAction=self.m_CurrentExecutedAction+1
				end

				for i=1, (#self.m_Actions) do
					if self.m_Actions[i]:IsActive() or self.m_Actions[i]:MustUpdate() then
						self.m_Actions[i]:Update(ElapsedTime)
					end
				end
			end
		end
	end

	function CUABCinematicsActionManager:LoadXML(Filename)
		self.m_MaxTime=0
		self.m_CurrentTime=0
		self.m_Actions={}
		self.m_CurrentAction=1
		self.m_CurrentExecutedAction=1
		local l_XMLTreeNode=CXMLTreeNode()
		local l_Loaded=l_XMLTreeNode:load_file(Filename)
		
		if l_Loaded then
			for i=0, l_XMLTreeNode:get_num_children() do
				local l_Atts=l_XMLTreeNode:get_child(i)
				local l_ElemName=l_Atts:get_name()
				
				if l_ElemName=="on" or
					l_ElemName=="after_last_action" then
					local l_ActionName=l_Atts:get_psz_property("action", "",false)
					local l_Action=nil
					
					if l_ActionName=="set_camera" then
						l_Action=CUABCinematicsActionSetCamera(l_Atts)
					end
					
					if l_Action~=nil then
						if l_ElemName=="after_last_action" then
							local l_LastActionTime=0
							local l_LastActionDuration=0
							
							if (#self.m_Actions)>0 then
								l_LastActionTime=self.m_Actions[(#self.m_Actions)]:GetTime() 
								l_LastActionDuration=self.m_Actions[(#self.m_Actions)]:GetDuration()
							end
							l_Action:SetTime(l_Action:GetTime()+l_LastActionTime+l_LastActionDuration)
							self.m_MaxTime=l_Action:GetTime()+l_LastActionTime+l_LastActionDuration
						end
						table.insert(self.m_Actions, l_Action)
						--sort actions by time
					end
				end
			end
		else
			utils_log("File '"..Filename.."'not correctly loaded")
		end
	end
--end