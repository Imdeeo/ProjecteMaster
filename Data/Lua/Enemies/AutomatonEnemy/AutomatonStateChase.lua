function ChaseFirstAutomaton(args)
	utils_log("ChaseFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 2
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_TotalNodes = 0.0
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_LastPositionPlayer = nil
	l_Enemy.m_IsCorrected = false
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
	
	if l_Enemy:PlayerVisible(l_Owner) then
		if l_Enemy:CheckPlayerDistance(l_Enemy.m_DistanceToKill) then
			l_Enemy.m_State = "attack"
		else
			l_Enemy.m_LastPositionPlayer = nil
			l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime

			local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed

			if l_PercentRotation > 1.0 then
				l_PercentRotation = 1.0
				l_Enemy.m_TimerRotation = 0.0
			end 
			
			l_Enemy:EnemyWalk(l_PlayerPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)
		end
	else
		if l_Enemy.m_BlockingObjectName == nil and not l_Enemy.m_DetectedSound then
			l_Enemy.m_LastPositionPlayer = nil
			l_Enemy.m_State = "alert"
		else
			-- Obtenemos la ruta a seguir a partir de la posicion del enemigo y la ultima posicion del player conocida
			if l_Enemy.m_LastPositionPlayer == nil then
				l_Enemy.m_LastPositionPlayer = Vect3f(l_PlayerPos.x, l_PlayerPos.y, l_PlayerPos.z) 
				l_Enemy:SearchForPath(l_Enemy.m_LastPositionPlayer)
				
				local l_FirstPointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint)
				local l_SecondPointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint+1)
				
				local l_EnemyPos = l_Owner:get_position()
				local l_DistanceFirstToSecondPoint = l_EnemyPos:distance(l_FirstPointPos) + l_FirstPointPos:distance(l_SecondPointPos)
				local l_DistanceToSecondPoint = l_EnemyPos:distance(l_FirstPointPos)
				local l_RaycastData = RaycastData()
				local l_Hit = l_Enemy.m_PhysXManager:raycast(l_EnemyPos + l_Enemy.m_HeadOffset, l_SecondPointPos, l_Enemy.m_PhysXGroups, l_RaycastData)
				
				if l_Hit then
					utils_log("OBJETO: "..l_RaycastData.actor_name)
				else
					utils_log("No ha habido hit")
				end
				
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
					local l_PointPos = l_Enemy.m_PathFinding:get_point(l_Enemy.m_Name, l_Enemy.m_IndexPoint)

					l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
					local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed

					if l_PercentRotation > 1.0 then
						l_PercentRotation = 1.0
						l_Enemy.m_TimerRotation = 0
					end
					
					l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)
					
					-- Si la distancia entre el enemy y el punto es menor de 1 pasamos al siguiente punto
					local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
					if l_Distance <= l_Enemy.m_DistanceToChangeNodeRunning then
						if l_Enemy:IncrementPathPointIndex() == false then
							-- no se ha podido pasar al siguiente punto porque era el ultimo, por tanto pasamos a alert ya que hemos perdido al player
							l_Enemy.m_State = "alert"
						end
					end
				end
			else
				l_Enemy.m_State = "alert"
			end
		end
	end
end

function ChaseEndAutomaton(args)
	utils_log("ChaseEnd")
end

function ChaseToAlertConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "alert"
end

function ChaseToAttackConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "attack"
end