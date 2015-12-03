#pragma once

#ifndef INC_FRUSTUM_H_
#define INC_FRUSTUM_H_


#include "Math\Matrix44.h"
#include "Math\Vector3.h"

class CFrustum 
{
private:
	float m_Proj[16];
	float m_Modl[16];
	float m_Clip[16];
	float m_Frustum[6][4];
public:

	void Update(const Mat44f &ViewProj);
	bool SphereVisible(const Vect3f &Center, float Radius) const;
	bool BoxVisible(const Vect3f &Max, const Vect3f &Min) const;
};

#endif
