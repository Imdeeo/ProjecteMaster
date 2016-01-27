#ifndef QUATN_H
#define QUATN_H

#include "Math\MathTypes.h"
#include "Math\MathUtils.h"

template<typename T>
class Quatn
{
public:
	//------------------
	// DATOS PUBLICOS
	//------------------
	T x, y, z, w;

public:
	Quatn(){}
	Quatn(T qx, T qy,T qz,T qw) : x(qx), y(qy), z(qz), w(qw)
	{
	}
};

typedef Quatn<float> Quatf;
typedef Quatn<double> Quatd;
typedef Quatn<int32_t>  Quati;
typedef Quatn<uint32_t> Quatu;
typedef Quatn<uint8_t>  Quatu8;
typedef Quatn<uint16_t> Quatw;

extern const Quatf qfIDENTITY;
extern const Quatf qfINVALID;

extern const Quatd qdIDENTITY;
extern const Quatd qdINVALID;

extern const Transformf quatposfIDENTITY;  
extern const Transformd quatposdIDENTITY;

#endif //QUATN_H