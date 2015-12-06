#ifndef QUATN_H
#define QUATN_H

#include "Math\MathTypes.h"
#include "Math\MathUtils.h"
#include "cal3d\quaternion.h"

template<typename T>
class Quatn:public CalQuaternion
{
public:
	Quatn(T qx,T qy,T qz,T qw):CalQuaternion(qx,qy,qz,qw)
	{
	}
};

extern const Quatf qfIDENTITY;
extern const Quatf qfINVALID;

extern const Quatd qdIDENTITY;
extern const Quatd qdINVALID;

extern const Transformf quatposfIDENTITY;  
extern const Transformd quatposdIDENTITY;

#endif //QUATN_H