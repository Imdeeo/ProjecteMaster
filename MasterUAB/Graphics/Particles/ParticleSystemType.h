#ifndef PARTICLE_SYSTEM_TYPE_H
#define PARTICLE_SYSTEM_TYPE_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"

#include <string>
#include <vector>

class CMaterial;
class CXMLTreeNode;

class CParticleSystemType : public CNamed
{
private:
	struct ControlPointColor
	{
		Vect2f m_Time;
		CColor m_Color1, m_Color2;
	};

	struct ControlPointSize
	{
		Vect2f m_Time, m_Size;
	};	
public:
	CParticleSystemType(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemType(void);
	void Destroy();

	UAB_BUILD_GET_SET(CMaterial *, Material);
	UAB_BUILD_GET_SET(int, NumFrames);
	UAB_BUILD_GET_SET(float, TimePerFrame);
	UAB_BUILD_GET_SET(bool, LoopFrames);
	UAB_BUILD_GET_SET(bool, EmitAbsolute);
	UAB_BUILD_GET_SET(float, StartingDirectionAngle);
	UAB_BUILD_GET_SET(float, StartingAccelerationAngle);
	UAB_BUILD_GET_SET(Vect2f, Size);
	UAB_BUILD_GET_SET(Vect2f, EmitRate);
	UAB_BUILD_GET_SET(Vect2f, AwakeTime);
	UAB_BUILD_GET_SET(Vect2f, SleepTime);
	UAB_BUILD_GET_SET(Vect2f, Life);
	UAB_BUILD_GET_SET(Vect2f, StartingAngle);
	UAB_BUILD_GET_SET(Vect2f, StartingAngularSpeed);
	UAB_BUILD_GET_SET(Vect2f, AngularAcceleration);
	UAB_BUILD_GET_SET(Vect3f, StartingSpeed1);
	UAB_BUILD_GET_SET(Vect3f, StartingSpeed2);
	UAB_BUILD_GET_SET(Vect3f, StartingAcceleration1);
	UAB_BUILD_GET_SET(Vect3f, StartingAcceleration2);
	UAB_BUILD_GET_SET(CColor, Color1);
	UAB_BUILD_GET_SET(CColor, Color2);

	std::vector<ControlPointColor> m_ControlPointColors;
	std::vector<ControlPointSize> m_ControlPointSizes;

	CEmptyPointerClass* GetLuaNumFrames()
	{
		return (CEmptyPointerClass*)&m_NumFrames;
	}

	CEmptyPointerClass* GetLuaTimePerFrame()
	{
		return (CEmptyPointerClass*)&m_TimePerFrame;
	}

	CEmptyPointerClass* GetLuaLoopFrames()
	{
		return (CEmptyPointerClass*)&m_LoopFrames;
	}

	CEmptyPointerClass* GetLuaEmitAbsolute()
	{
		return (CEmptyPointerClass*)&m_EmitAbsolute;
	}

	CEmptyPointerClass* GetLuaStartingDirectionAngle()
	{
		return (CEmptyPointerClass*)&m_StartingDirectionAngle;
	}

	CEmptyPointerClass* GetLuaStartingAccelerationAngle()
	{
		return (CEmptyPointerClass*)&m_StartingAccelerationAngle;
	}

	CEmptyPointerClass* GetLuaSize()
	{
		return (CEmptyPointerClass*)&m_Size;
	}

	CEmptyPointerClass* GetLuaEmitRate()
	{
		return (CEmptyPointerClass*)&m_EmitRate;
	}

	CEmptyPointerClass* GetLuaAwakeTime()
	{
		return (CEmptyPointerClass*)&m_AwakeTime;
	}

	CEmptyPointerClass* GetLuaSleepTime()
	{
		return (CEmptyPointerClass*)&m_SleepTime;
	}

	CEmptyPointerClass* GetLuaLife()
	{
		return (CEmptyPointerClass*)&m_Life;
	}
	
	CEmptyPointerClass* GetLuaStartingAngle()
	{
		return (CEmptyPointerClass*)&m_StartingAngle;
	}

	CEmptyPointerClass* GetLuaStartingAngularSpeed()
	{
		return (CEmptyPointerClass*)&m_StartingAngularSpeed;
	}

	CEmptyPointerClass* GetLuaAngularAcceleration()
	{
		return (CEmptyPointerClass*)&m_AngularAcceleration;
	}

	CEmptyPointerClass* GetLuaStartingSpeed1()
	{
		return (CEmptyPointerClass*)&m_StartingSpeed1;
	}

	CEmptyPointerClass* GetLuaStartingSpeed2()
	{
		return (CEmptyPointerClass*)&m_StartingSpeed2;
	}

	CEmptyPointerClass* GetLuaStartingAcceleration1()
	{
		return (CEmptyPointerClass*)&m_StartingAcceleration1;
	}

	CEmptyPointerClass* GetLuaStartingAcceleration2()
	{
		return (CEmptyPointerClass*)&m_StartingAcceleration2;
	}
	
	CEmptyPointerClass* GetLuaColor1()
	{
		return (CEmptyPointerClass*)&m_Color1;
	}

	CEmptyPointerClass* GetLuaColor2()
	{
		return (CEmptyPointerClass*)&m_Color2;
	}
};

#endif //PARTICLE_SYSTEM_TYPE_H