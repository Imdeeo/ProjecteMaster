function table_length(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

function swap_xz(_vector)
	local aux = _vector.z
	_vector.z = _vector.x
	_vector.x = aux
end

function swap_yz(_vector)
	local aux = _vector.z
	_vector.z = _vector.y
	_vector.y = aux
end

function swap_xy(_vector)
	local aux = _vector.y
	_vector.y = _vector.x
	_vector.x = aux
end

function mirror_quat_in_x(_quat)
	_quat.y = _quat.y * (-1.0)
	_quat.z = _quat.z * (-1.0)
end

function mirror_quat_in_y(_quat)
	_quat.x = _quat.x * (-1.0)
	_quat.z = _quat.z * (-1.0)
end

function mirror_quat_in_z(_quat)
	_quat.y = _quat.y * (-1.0)
	_quat.x = _quat.x * (-1.0)
end