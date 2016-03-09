#ifndef PARTICLE_SYSTEM_TYPE_H
#define PARTICLE_SYSTEM_TYPE_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"

#include <string>

class CMaterial;
class CXMLTreeNode;

class CParticleSystemType : public CNamed
{
private:
	int m_NumFrames;	
	float m_TimePerFrame;
	bool m_LoopFrames;
	bool m_EmitAbsolute;
	float m_StartingDirectionAngle, m_StartingAccelerationAngle;
	Vect2f m_Size;
	Vect2f m_EmitRate, m_AwakeTime, m_SleepTime, m_Life;
	Vect2f m_StartingAngle, m_StartingAngularSpeed, m_AngularAcceleration;
	Vect3f m_StartingSpeed1, m_StartingSpeed2;
	Vect3f m_StartingAcceleration1, m_StartingAcceleration2;
	CColor m_Color1, m_Color2;

public:
	CParticleSystemType(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemType(void);
	void Destroy();

	UAB_BUILD_GET_SET(CMaterial *, Material);
	UAB_GET_PROPERTY(int, NumFrames);
	UAB_GET_PROPERTY(float, TimePerFrame);
	UAB_GET_PROPERTY(bool, LoopFrames);
	UAB_GET_PROPERTY(bool, EmitAbsolute);
	UAB_GET_PROPERTY(float, StartingDirectionAngle);
	UAB_GET_PROPERTY(float, StartingAccelerationAngle);
	UAB_GET_PROPERTY(Vect2f, Size);
	UAB_GET_PROPERTY(Vect2f, EmitRate);
	UAB_GET_PROPERTY(Vect2f, AwakeTime);
	UAB_GET_PROPERTY(Vect2f, SleepTime);
	UAB_GET_PROPERTY(Vect2f, Life);
	UAB_GET_PROPERTY(Vect2f, StartingAngle);
	UAB_GET_PROPERTY(Vect2f, StartingAngularSpeed);
	UAB_GET_PROPERTY(Vect2f, AngularAcceleration);
	UAB_GET_PROPERTY(Vect3f, StartingSpeed1);
	UAB_GET_PROPERTY(Vect3f, StartingSpeed2);
	UAB_GET_PROPERTY(Vect3f, StartingAcceleration1);
	UAB_GET_PROPERTY(Vect3f, StartingAcceleration2);
	UAB_GET_PROPERTY(CColor, Color1);
	UAB_GET_PROPERTY(CColor, Color2);	
};

#endif //PARTICLE_SYSTEM_TYPE_H