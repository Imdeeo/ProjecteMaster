#pragma once

#include "Math\Color.h"
#include "Math\Vector3.h"

struct SPositionOrientation
{
	Vect3f Position;
	float Yaw, Pitch, Roll;

	SPositionOrientation()
		: Position(0,0,0)
		, Yaw(0)
		, Pitch(0)
		, Roll(0)
	{}
};

