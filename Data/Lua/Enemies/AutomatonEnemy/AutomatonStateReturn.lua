function ReturnFirstAutomaton(args)
	utils_log("ReturnFirst")
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(2,0.5)
	l_Owner:blend_cycle(1,1.0,0.5)
	
	local l_Enemy = args["self"]
	l_Enemy.m_TimerRotation = 0.0
end

function ReturnUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]	
	
	if l_Enemy:PlayerVisible(l_Owner) then
		l_Enemy.m_State = "chase"	
	else
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
			
		if l_PercentRotation > 1.0 then
			l_PercentRotation = 1.0
			l_Enemy.m_TimerRotation = 0.0
		end
		
		if l_Enemy.m_BlockingObjectName == nil then			
			l_Enemy:EnemyWalk(l_Enemy.m_DefaultPosition, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
			
			-- Si la distancia entre el enemy y el punto es menor de un valor predeterminado pasamos al siguiente punto
			local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_Enemy.m_DefaultPosition)	
			
			if l_Distance < 0.5 then
				if l_Enemy.m_Patrol  then
					l_Enemy.m_State = "patrol"
				else
					l_Enemy.m_State = "idle"
				end
			end
		else
			-- Obtenemos la ruta a seguir a partir de la posicion del punto inicial y el player	
			local l_TotalNodes = l_Enemy.m_PathFindig:search_for_path(l_Enemy.m_RenderableObject:get_position(),l_Enemy.m_DefaultPosition)
			
			-- Comprobamos si ha podido establecer una ruta
			if l_TotalNodes > 0 then
				-- Actualizamos la posicion del enemigo
				local l_PointPos = l_Enemy.m_PathFindig:get_actual_pos()				
				
				l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
				
				-- Si la distancia entre el enemy y el punto es menor de un valor predeterminado pasamos al siguiente punto
				local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
				if l_Distance <= 2.0 then
					l_Enemy.m_PathFindig:increment_actual_point()
				end
			else
				l_Enemy.m_State = "idle"
			end			
		end
	end
end

function ReturnEndAutomaton(args)
	utils_log("ReturnEnd")
end

function ReturnToIdleConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function ReturnToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end