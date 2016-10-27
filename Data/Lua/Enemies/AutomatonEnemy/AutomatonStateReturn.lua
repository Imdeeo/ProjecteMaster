function ReturnFirstAutomaton(args)
	utils_log("ReturnFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)

	local l_SoundSync = CSoundSynchronizer(l_Enemy, l_Owner, 4)
	l_SoundSync.m_EventsPerCycle = 2
	l_SoundSync.m_StartSequenceEvent = l_Enemy.m_StartPatrolEvent
	l_SoundSync.m_StopSequenceEvent = l_Enemy.m_StopPatrolEvent
	l_SoundSync.m_Offset = 0.31
	l_Enemy.m_SoundSync = l_SoundSync
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_TotalNodes = 0.0
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_TimerRotation2 = 0.0
	l_Enemy.m_IsReturn = false
	l_Enemy.m_LastPositionPlayer = nil
	l_Enemy.m_IsCorrected = false
end

function ReturnUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]	
	
	l_Enemy.m_SoundSync:Sync(_ElapsedTime)
	if l_Enemy:PlayerVisible(l_Owner) or l_Enemy:DetectPlayerNoise(1) then
		l_Enemy.m_State = "chase"
	else
		-- Obtenemos la ruta a seguir a partir de la posicion del punto inicial y la posicion actual
		if not l_Enemy.m_IsReturn then
			local pos = l_Enemy.m_RenderableObject:get_position()
			l_Enemy:SearchForPath(l_Enemy.m_DefaultPosition)
			l_Enemy.m_IsReturn = true
			
			local l_FirstPointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint)
			local l_SecondPointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint+1)
			
			local l_EnemyPos = l_Owner:get_position()
			local l_DistanceFirstToSecondPoint = l_EnemyPos:distance(l_FirstPointPos) + l_FirstPointPos:distance(l_SecondPointPos)
			local l_DistanceToSecondPoint = l_EnemyPos:distance(l_FirstPointPos)
			local l_RaycastData = RaycastData()
			local l_Hit = l_Enemy.m_PhysXManager:raycast(l_EnemyPos + l_Enemy.m_HeadOffset, l_SecondPointPos, l_Enemy.m_PhysXGroups, l_RaycastData)
			
			if l_DistanceToSecondPoint < l_DistanceFirstToSecondPoint and not l_Hit then
				l_Enemy:IncrementPathPointIndex()
			end
		end
		
		-- Comprobamos si ha podido establecer una ruta
		if l_Enemy.m_TotalNodes > 0 then
			-- Actualizamos la posicion del enemigo
			if not l_Enemy.m_IsCorrected then				
				local l_PointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint)
				local l_Direction = (l_PointPos - l_Owner:get_position())
				l_Direction.y = 0
				l_Direction:normalize(1)
				
				local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Owner:get_rotation():get_forward_vector(), l_Direction)
				
				if angle_to_turn ~= 0.0 then
					l_Enemy:EnemyRotation(angle_to_turn, _ElapsedTime)
				else
					l_Enemy.m_IsCorrected = true
				end
			else
				if l_Enemy.m_IndexPoint < l_Enemy.m_TotalNodes then
					l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
					local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularWalkSpeed
						
					if l_PercentRotation > 1.0 then
						l_PercentRotation = 1.0
						l_Enemy.m_TimerRotation = 0.0
					end
				
					local l_PointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint)
					l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
					
					-- Si la distancia entre el enemy y el punto es menor de un valor predeterminado pasamos al siguiente punto
					local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
					if l_Distance <= l_Enemy.m_DistanceToChangeNodeWalking then
						if l_Enemy:IncrementPathPointIndex() == false then
							l_Enemy.m_IndexPoint = l_Enemy.m_IndexPoint + 1
						end
					end
				else
					local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Owner:get_rotation():get_forward_vector(), l_Enemy.m_DefaultRotation:get_forward_vector())
					
					if angle_to_turn ~= 0.0 then
						l_Enemy:EnemyRotation(angle_to_turn, _ElapsedTime)
					else
						l_Enemy.m_State = "idle"
					end
				end
			end
		else
			l_Enemy.m_State = "idle"
		end
	end
end

function ReturnEndAutomaton(args)
	utils_log("ReturnEnd")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	l_Enemy.m_SoundManager:play_event(l_Enemy.m_StopPatrolEvent, l_Owner)
end

function ReturnToIdleConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function ReturnToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end
