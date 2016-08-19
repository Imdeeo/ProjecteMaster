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
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)
	l_Enemy.m_Timer = l_Enemy.m_Timer + _ElapsedTime
	l_Enemy:LoseSanity(l_Distance)
	
	if l_Enemy:PlayerVisible(l_Owner) then
		l_Enemy.m_LastPositionPlayer = nil
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime
		
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
			
		if l_PercentRotation > 1.0 then
			l_PercentRotation = 1.0
			l_Enemy.m_TimerRotation = 0.0
		end 
		
		l_Enemy:EnemyWalk(l_PlayerPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)		
		
		-- Si la distancia entre el enemy y el player es menor a lo establecido pasamos a attack
		if l_Distance < l_Enemy.m_DistanceToKill then
			l_Enemy.m_State = "attack"
		end		
	else
		if l_Enemy.m_BlockingObjectName == nil and l_Enemy.m_IsChasing then
			l_Enemy.m_LastPositionPlayer = nil
			l_Enemy.m_State = "alert"
		else
			-- Obtenemos la ruta a seguir a partir de la posicion del enemigo y la ultima posicion del player conocida
			if l_Enemy.m_LastPositionPlayer == nil then
				l_Enemy.m_LastPositionPlayer = Vect3f(l_PlayerPos.x, l_PlayerPos.y, l_PlayerPos.z) 
				l_Enemy.m_TotalNodes = l_Enemy.m_PathFindig:search_for_path(l_Enemy.m_RenderableObject:get_position(),l_Enemy.m_LastPositionPlayer)
			end	
			
			-- Comprobamos si ha podido establecer una ruta
			if l_Enemy.m_TotalNodes > 0 then
				-- Actualizamos la posicion del enemigo
				local l_PointPos = l_Enemy.m_PathFindig:get_actual_pos()
				
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
				local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
					
				if l_PercentRotation > 1.0 then
					l_PercentRotation = 1.0
					l_Enemy.m_TimerRotation = 0
				end 
				
				l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)
				
				-- Si la distancia entre el enemy y el punto es menor de 1 pasamos al siguiente punto
				local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
				if l_Distance <= 0.5 then
					if l_Enemy.m_PathFindig:increment_actual_point() == false then
						-- no se ha podido pasar al siguiente punto porque era el ultimo, por tanto pasamos a alert ya que hemos perdido al player
						l_Enemy.m_State = "alert"
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