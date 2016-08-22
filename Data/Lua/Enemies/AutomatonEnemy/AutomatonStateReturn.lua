function ReturnFirstAutomaton(args)
	utils_log("ReturnFirst")
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]
	
	l_Owner:clear_cycle(l_Enemy.m_ActualAnimation,0.5)
	l_Enemy.m_ActualAnimation = 1
	l_Owner:blend_cycle(l_Enemy.m_ActualAnimation,1.0,0.5)
		
	l_Enemy.m_Velocity = Vect3f(0,0,0)
	l_Enemy.m_TotalNodes = 0.0
	l_Enemy.m_TimerRotation = 0.0
	l_Enemy.m_TimerRotation2 = 0.0
	l_Enemy.m_IsReturn = false
	l_Enemy.m_LastPositionPlayer = nil
	l_Enemy.m_IsChasing = false
end

function ReturnUpdateAutomaton(args, _ElapsedTime)
	local l_Owner = args["owner"]
	local l_Enemy = args["self"]	
	
	if l_Enemy:PlayerVisible(l_Owner) then
		l_Enemy.m_IsChasing = true
		l_Enemy.m_State = "chase"	
	elseif l_Enemy:DetectPlayerNoise(1) then
		l_Enemy.m_State = "chase"
	else
		l_Enemy.m_TimerRotation = l_Enemy.m_TimerRotation + _ElapsedTime	
		local l_PercentRotation = l_Enemy.m_TimerRotation / l_Enemy.m_AngularWalkSpeed
			
		if l_PercentRotation > 1.0 then
			l_PercentRotation = 1.0
			l_Enemy.m_TimerRotation = 0.0
		end

		-- Obtenemos la ruta a seguir a partir de la posicion del punto inicial y la posicion actual
		if not l_Enemy.m_IsReturn then
			local pos = l_Enemy.m_RenderableObject:get_position()
			l_Enemy.m_TotalNodes = l_Enemy.m_PathFindig:search_for_path(l_Enemy.m_RenderableObject:get_position(),l_Enemy.m_DefaultPosition)
			l_Enemy.m_IsReturn = true
		end
		
		-- Comprobamos si ha podido establecer una ruta
		if l_Enemy.m_TotalNodes > 0 then
			-- Actualizamos la posicion del enemigo
			local l_PointPos = l_Enemy.m_PathFindig:get_actual_pos()
			l_Enemy:EnemyWalk(l_PointPos, l_Enemy.m_WalkSpeed, l_PercentRotation, _ElapsedTime)
			
			-- Si la distancia entre el enemy y el punto es menor de un valor predeterminado pasamos al siguiente punto
			local l_Distance = l_Enemy.m_RenderableObject:get_position():distance(l_PointPos)	
			if l_Distance <= l_Enemy.m_DistanceToChangeNodeWalking then
				if l_Enemy.m_PathFindig:increment_actual_point() == false then
					local angle_to_turn = l_Enemy:CalculateAngleRotation(l_Owner:get_rotation():get_forward_vector(), l_Enemy.m_DefaultForward)
					if angle_to_turn ~= nil and (angle_to_turn > 0.75 or angle_to_turn < 0.65) then
						l_Enemy.m_TimerRotation2 = l_Enemy.m_TimerRotation2 + _ElapsedTime	
						l_PercentRotation = l_Enemy.m_TimerRotation2 / l_Enemy.m_AngularWalkSpeed * 5
							
						if l_PercentRotation > 1.0 then
							l_PercentRotation = 1.0
						end
						l_Enemy:EnemyRotation(angle_to_turn, l_PercentRotation)
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
end

function ReturnToIdleConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "idle"
end

function ReturnToChaseConditionAutomaton(args)
	local l_Enemy = args["self"]
	return l_Enemy.m_State == "chase"
end