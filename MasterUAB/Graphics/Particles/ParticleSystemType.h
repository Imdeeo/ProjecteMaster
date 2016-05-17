#ifndef PARTICLE_SYSTEM_TYPE_H
#define PARTICLE_SYSTEM_TYPE_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"
#include "Materials\MaterialManager.h"
#include "Texture\Texture.h"

#include <string>
#include <vector>

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
	void Save(FILE* _File);

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
	int GetColorsControlPointSize() { return m_ControlPointColors.size(); }
	int GetSizesControlPointSize() { return m_ControlPointSizes.size(); }

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

	CEmptyPointerClass* GetLuaSize(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Size[index];
	}

	CEmptyPointerClass* GetLuaEmitRate(int index = 0)
	{
		return (CEmptyPointerClass*)&m_EmitRate[index];
	}

	CEmptyPointerClass* GetLuaAwakeTime(int index = 0)
	{
		return (CEmptyPointerClass*)&m_AwakeTime[index];
	}

	CEmptyPointerClass* GetLuaSleepTime(int index = 0)
	{
		return (CEmptyPointerClass*)&m_SleepTime[index];
	}

	CEmptyPointerClass* GetLuaLife(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Life[index];
	}
	
	CEmptyPointerClass* GetLuaStartingAngle(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingAngle[index];
	}

	CEmptyPointerClass* GetLuaStartingAngularSpeed(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingAngularSpeed[index];
	}

	CEmptyPointerClass* GetLuaAngularAcceleration(int index = 0)
	{
		return (CEmptyPointerClass*)&m_AngularAcceleration[index];
	}

	CEmptyPointerClass* GetLuaStartingSpeed1(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingSpeed1[index];
	}

	CEmptyPointerClass* GetLuaStartingSpeed2(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingSpeed2[index];
	}

	CEmptyPointerClass* GetLuaStartingAcceleration1(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingAcceleration1[index];
	}

	CEmptyPointerClass* GetLuaStartingAcceleration2(int index = 0)
	{
		return (CEmptyPointerClass*)&m_StartingAcceleration2[index];
	}
	
	CEmptyPointerClass* GetLuaColor1()
	{
		return (CEmptyPointerClass*)&m_Color1;
	}

	CEmptyPointerClass* GetLuaColor2()
	{
		return (CEmptyPointerClass*)&m_Color2;
	}

	CEmptyPointerClass* GetLuaCPSize(int index = 0, int index2 = 0)
	{
		return (CEmptyPointerClass*)&(m_ControlPointSizes[index].m_Size[index2]);
	}
	
	CEmptyPointerClass* GetLuaCPSizeTime(int index = 0, int index2 = 0)
	{
		return (CEmptyPointerClass*)&(m_ControlPointSizes[index].m_Time[index2]);
	}

	CEmptyPointerClass* GetLuaCPColor1(int index = 0)
	{
		return (CEmptyPointerClass*)&(m_ControlPointColors[index].m_Color1);
	}

	CEmptyPointerClass* GetLuaCPColor2(int index = 0)
	{
		return (CEmptyPointerClass*)&(m_ControlPointColors[index].m_Color2);
	}

	CEmptyPointerClass* GetLuaCPColorTime(int index = 0, int index2 = 0)
	{
		return (CEmptyPointerClass*)&(m_ControlPointColors[index].m_Time[index2]);
	}
};

#endif //PARTICLE_SYSTEM_TYPE_H