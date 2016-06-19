#ifndef MANCHAS_SYSTEM_TYPE_H
#define MANCHAS_SYSTEM_TYPE_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Matrix44.h"
#include "Math\Color.h"
#include "Materials\MaterialManager.h"
#include "Texture\Texture.h"

#include <string>
#include <vector>

class CXMLTreeNode;

class CManchasSystemType : public CNamed
{
public:
	CManchasSystemType(CXMLTreeNode &TreeNode);
	virtual ~CManchasSystemType(void);
	void Destroy();
	//void Save(FILE* _File);

	UAB_BUILD_GET_SET(CMaterial *, Material);
	UAB_BUILD_GET_SET(Vect2f, EmitTime);
	UAB_BUILD_GET_SET(Vect2f, Life);
	UAB_BUILD_GET_SET(Vect2f, Opacity);
	UAB_BUILD_GET_SET(Vect2f, Size);
	UAB_BUILD_GET_SET(Vect2f, SizeSpeed);
	UAB_BUILD_GET_SET(CColor, Color1);
	UAB_BUILD_GET_SET(CColor, Color2);

	/*CEmptyPointerClass* GetLuaSize1(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Size1[index];
	}

	CEmptyPointerClass* GetLuaSize2(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Size2[index];
	}

	CEmptyPointerClass* GetLuaEmitRate(int index = 0)
	{
		return (CEmptyPointerClass*)&m_EmitTime[index];
	}
	
	CEmptyPointerClass* GetLuaLife(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Life[index];
	}*/
		
};

#endif //MANCHAS_SYSTEM_TYPE_H