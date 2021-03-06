#ifndef BILBOARD_SYSTEM_TYPE_H
#define BILBOARD_SYSTEM_TYPE_H

#include "Utils.h"
#include "Utils\Named.h"
#include "Utils\LevelInfo.h"
#include "XML\tinyxml2.h"

#include <string>
#include <vector>

class CMaterial;
class CLevel;

class CBilboardSystemType : public CNamed, CLevelInfo
{
public:
	CBilboardSystemType(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CBilboardSystemType(void);
	void Destroy();
	void Save(FILE* _File);

	UAB_BUILD_GET_SET(CMaterial *, Material);
	UAB_BUILD_GET_SET(int, NumFrames);
	UAB_BUILD_GET_SET(float, TimePerFrame);

	CEmptyPointerClass* GetLuaNumFrames()
	{
		return (CEmptyPointerClass*)&m_NumFrames;
	}

	CEmptyPointerClass* GetLuaTimePerFrame()
	{
		return (CEmptyPointerClass*)&m_TimePerFrame;
	}
};

#endif //BILBOARD_SYSTEM_TYPE_H