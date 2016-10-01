#ifndef BILBOARD_SYSTEM_INSTANCE_H
#define BILBOARD_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "RenderableObjects\VertexTypes.h"
#include "XML\tinyxml2.h"

#include <random>

class CBilboardSystemType;
class CRenderableVertexs;

class CBilboardSystemInstance : public CRenderableObject
{
private:
	unsigned short l_IndexType = 16;

	struct BilboardData
	{
		Vect3f Position;
		int CurrentFrame;
		float TimeToNextFrame;
	};

	int m_ActiveBilboards;
	BilboardData m_BilboardData[MAX_BILBOARDS_PER_INSTANCE];
	MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX m_BilboardRenderableData[MAX_BILBOARDS_PER_INSTANCE];
	CRenderableVertexs *m_RenderableVertex;
public:
	CBilboardSystemInstance(){};
	CBilboardSystemInstance(tinyxml2::XMLElement* TreeNode);
	virtual ~CBilboardSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(CBilboardSystemType *, Type);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
	
	CEmptyPointerClass* GetLuaEmissionBoxPosition(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Position[index];
	}
	UAB_BUILD_GET_SET(bool, Start);
	UAB_BUILD_GET_SET(float, Size);
	UAB_BUILD_GET_SET(float, offsetSize);
	UAB_BUILD_GET_SET(CColor, Color);
	CEmptyPointerClass* GetLuaSize()
	{
		return (CEmptyPointerClass*)&m_Size;
	}
	CEmptyPointerClass* GetLuaSizeOffset()
	{
		return (CEmptyPointerClass*)&m_offsetSize;
	}
	CEmptyPointerClass* GetLuaColor()
	{
		return (CEmptyPointerClass*)&m_Color;
	}
};

#endif //BILBOARD_SYSTEM_INSTANCE_H