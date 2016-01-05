fn circumcenter p1 p2 p3 =
(
    BC = distance p2 p3
    CA = distance p3 p1
    AB = distance p1 p2

    baryCoords = [ (BC^2 * (CA^2 + AB^2 - BC^2)), (CA^2 * (AB^2 + BC^2 - CA^2)), (AB^2 * (BC^2 + CA^2 - AB^2)) ]
    triArea = baryCoords.x + baryCoords.y + baryCoords.z
    baryCoords /= triArea

    baryCoords.x * p1 + baryCoords.y * p2 + baryCoords.z * p3
)

fn mat3xvect3 mat vect =
( 
    local res = [0, 0, 0]
	local x = vect.x
	local y = vect.y
	local z = vect.z
    res.x = (mat.row1.x*x) + (mat.row1.y*y) + (mat.row1.z*z)
    res.y = (mat.row2.x*x) + (mat.row2.y*y) + (mat.row2.z*z)
    res.z = (mat.row3.x*x) + (mat.row3.y*y) + (mat.row3.z*z)
    return res
)

fn mat4xvect3 mat vect w =
( 
    -- w=1 -> Translate // w=0 -> Rotate
    local res = [0, 0, 0]
	local x = vect.x
	local y = vect.y
	local z = vect.z
    res.x = (mat.row1.x*x) + (mat.row2.x*y) + (mat.row3.x*z) + (mat.row4.x*w)
    res.y = (mat.row1.y*x) + (mat.row2.y*y) + (mat.row3.y*z) + (mat.row4.y*w)
    res.z = (mat.row1.z*x) + (mat.row2.z*y) + (mat.row3.z*z) + (mat.row4.z*w)
    return res
)
