#ifndef MANCHAS_SYSTEM_TYPE_H
#define MANCHAS_SYSTEM_TYPE_H

#include "Utils\Named.h"
#include "Utils\LevelInfo.h"
#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"
#include "Materials\MaterialManager.h"
#include "Texture\Texture.h"
#include "XML\tinyxml2.h"

#include <string>
#include <vector>

class CLevel;

class CManchasSystemType : public CNamed, public CLevelInfo
{
public:
	CManchasSystemType(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CManchasSystemType(void);
	void Destroy();
	void Save(FILE* _File);

	UAB_BUILD_GET_SET(CMaterial *, Material);
	UAB_BUILD_GET_SET(int, Frames);
	UAB_BUILD_GET_SET(Vect2f, EmitTime);
	UAB_BUILD_GET_SET(Vect2f, Life);
	UAB_BUILD_GET_SET(Vect2f, Opacity);
	UAB_BUILD_GET_SET(Vect2f, Size);
	UAB_BUILD_GET_SET(Vect2f, SizeSpeed);
	UAB_BUILD_GET_SET(CColor, Color1);
	UAB_BUILD_GET_SET(CColor, Color2);

	CEmptyPointerClass* GetLuaFrames()
	{
		return (CEmptyPointerClass*)&m_Frames;
	}
	CEmptyPointerClass* GetLuaEmitTime(int index = 0)
	{
		return (CEmptyPointerClass*)&m_EmitTime[index];
	}
	CEmptyPointerClass* GetLuaLife(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Life[index];
	}
	CEmptyPointerClass* GetLuaOpacity(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Opacity[index];
	}
	CEmptyPointerClass* GetLuaSize(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Size[index];
	}
	CEmptyPointerClass* GetLuaSizeSpeed(int index = 0)
	{
		return (CEmptyPointerClass*)&m_SizeSpeed[index];
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

#endif //MANCHAS_SYSTEM_TYPE_H