function FacingRaycast(_Offset, _Target, _Pos, _Distance)
	--utils_log("Pos: ".._Pos.x..", ".._Pos.y..", ".._Pos.z)
	--utils_log("Target: ".._Target.x..", ".._Target.y..", ".._Target.z)
	--utils_log("Offset: ".._Offset.x..", ".._Offset.y..", ".._Offset.z)
	--utils_log("Distance: "..(_Pos - _Target):length())
	return ((_Pos - _Target):length() < _Distance)
end
