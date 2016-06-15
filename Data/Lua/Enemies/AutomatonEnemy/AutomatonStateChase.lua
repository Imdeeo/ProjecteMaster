function ChaseFirstAutomaton(args)
	local l_Owner = args["owner"]
	l_Owner:clear_cycle(0,0.5)
	l_Owner:clear_cycle(1,0.5)
	l_Owner:blend_cycle(2,1.0,0.5)
	
	local l_Enemy = args["self"]
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_PathFindig:load_map("Data\\level_"..g_Engine:get_level_loaded().."\\pathfinding.xml")
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	
	if l_Enemy:PlayerVisible(l_Owner) then
		utils_log("TE HE VISTO!!!!")
				
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
			
		if l_PercentRotation > 1.0 then
			l_PercentRotation = 1.0
			l_Enemy.m_TimerRotation = 0.0
		end 
		
		l_Enemy:EnemyWalk(l_PlayerPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)
		
		-- Si la distancia entre el enemy y el player es menor a lo establecido pasamos a attack
		local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PlayerPos)	
		if l_Distance < l_Enemy.m_distance_to_kill then
			l_Enemy.m_State = "attack"
		end
		
	else
		if l_Enemy.m_BlockingObjectName == nil then
			utils_log("TODO DESPEJADO!!!!")
			l_Enemy.m_State = "alert"
		else
			utils_log("ALGO ME BLOQUEA EL CAMINO!!!!")
			
			-- Obtenemos la ruta a seguir a partir de la posicion del enemigo y el player	
			local l_TotalNodes = l_Enemy.m_PathFindig:search_for_path(l_Enemy.m_RenderableObject:get_position(),l_PlayerPos)
			utils_log("TOTAL NODES: "..l_TotalNodes)
			
			-- Comprobamos si ha podido establecer una ruta
			if l_TotalNodes > 0 then
				utils_log("Hay Nodos a seguir")
				-- Actualizamos la posicion del enemigo
				local l_PointPos = l_Enemy.m_PathFindig:get_actual_pos()
				
				l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
				local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularRunSpeed
					
				if l_PercentRotation > 1.0 then
					l_PercentRotation = 1.0
					l_Enemy.m_TimerRotation = 0.0
				end 
				
				l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_RunSpeed, l_PercentRotation, _ElapsedTime)
				
				-- Si la distancia entre el enemy y el punto es menor de 1 pasamos al siguiente punto
				local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
				if l_Distance <= 2.0 then
					l_Enemy.m_PathFindig:increment_actual_point()
				end
			else
				utils_log("FUERA DE MI ALCANCE!!!")
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