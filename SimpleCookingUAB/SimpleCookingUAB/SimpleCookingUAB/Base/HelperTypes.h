#ifndef HELPERTYPES_H
#define HELPERTYPES_H

#include "Math\Color.h"
#include "Math\Vector3.h"
#include "no_sillywarnings_please.h"

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

#endif