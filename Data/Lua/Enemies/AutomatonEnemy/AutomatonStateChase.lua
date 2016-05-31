function ChaseFirstAutomaton(args)
	local l_Owner = args["owner"]
	l_Owner:blend_cycle(0,1.0,0.1)
	local l_Enemy = m_CharacterManager.m_Enemics[1]	
	utils_log("LEVEL: "..g_Engine:get_level_loaded())
	l_Enemy.m_PathFindig:load_map("Data\\level_"..g_Engine:get_level_loaded().."\\pathfinding.xml")
end

function ChaseUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = m_CharacterManager.m_Enemics[1]		
	local l_PlayerPos = g_Player.m_RenderableObject:get_position()
	local l_EnemyPos = l_Owner:get_position()
	local l_Direction = 0.0
	
	-- Obtenemos la ruta a seguir a partir de la posicion del enemigo y el player	
	local l_TotalNodes = l_Enemy.m_PathFindig:search_for_path(l_Enemy.m_RenderableObject:get_position(),l_PlayerPos)
	utils_log("TOTAL NODES: "..l_TotalNodes)
	
	-- Comprobamos si ha podido establecer una ruta
	if l_TotalNodes > 0 then
		utils_log("Hay Nodos a seguir")
		-- Actualizamos la posicion del enemigo
		local l_PointPos = l_Enemy.m_PathFindig:get_actual_pos()
		l_Direction = (l_PointPos - l_EnemyPos):get_normalized(1)
		l_Owner:set_position(l_EnemyPos + (l_Direction * l_Enemy.m_Speed * _ElapsedTime))	
		utils_log("Nueva Posicion")
		
		-- Si la distancia entre el enemy y el punto es menor de 1 pasamos al siguiente punto
		local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
		if l_Distance >= 0.0 and l_Distance <= 1.0 then
			l_Enemy.m_PathFindig:increment_actual_point()
		end
		utils_log("Comprobando distancia")
	else
		utils_log("No hay nodos. A por EEEEELLLLL!!!")
		local l_PlayerPos = g_Player.m_RenderableObject:get_position()
		l_Direction = (l_PlayerPos - l_EnemyPos):get_normalized(1)	
		l_Owner:set_position(l_EnemyPos + (l_Direction * l_Enemy.m_Speed * _ElapsedTime))
	end
	
	-- Orientamos al enemy hacia el punto al que se dirige
	local l_EnemyForward = l_Owner:get_rotation():get_forward_vector():get_normalized(1)
	
	local l_Angle = l_EnemyForward * l_Direction
    if 1.0 - l_Angle < 0.01 then
      return
    end
	
    local angle_to_turn = math.acos(l_Angle)
    local cross = l_Direction ^ l_EnemyForward
    if cross.y < 0.0 then
      angle_to_turn = -angle_to_turn
    end
    local quat_to_turn = Quatf()
    quat_to_turn:quat_from_yaw_pitch_roll(angle_to_turn, 0.0, 0.0)
    local target_quat = l_Owner:get_rotation() * quat_to_turn
    l_Owner:set_rotation(target_quat)
end

function ChaseEndAutomaton(args)
	utils_log("ChaseEnd")
end

function ChaseToAlertConditionAutomaton()
end

function ChaseToAttackConditionAutomaton()
	local l_Enemy = m_CharacterManager.m_Enemics[1]	
	local l_Distance = g_Player.m_RenderableObject:get_position():distance(l_Enemy.m_RenderableObject:get_position())
	
	return l_Distance < l_Enemy.m_distance_to_kill
end