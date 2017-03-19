#ifndef BILBOARD_SYSTEM_INSTANCE_H
#define BILBOARD_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "RenderableObjects\VertexTypes.h"
#include "XML\tinyxml2.h"

#include <random>

class CBilboardSystemType;
class CRenderableVertexs;
class CLevel;

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
	BilboardData m_BilboardData[MAX_BILBOARDS_PER_INSTANCE];
	MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX m_BilboardRenderableData[MAX_BILBOARDS_PER_INSTANCE];
	CRenderableVertexs *m_RenderableVertex;
public:
	CBilboardSystemInstance(CLevel *_Level);
	CBilboardSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CBilboardSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(CBilboardSystemType *, Type);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	void Save(FILE* _File, std::string _layer);
	//void RenderDebug(CRenderManager *RM);
	std::string GetTipo(){ return "BilboardInstance"; };

	UAB_BUILD_GET_SET(bool, Start);
	UAB_BUILD_GET_SET(float, SizeX);
	UAB_BUILD_GET_SET(float, SizeY);
	UAB_BUILD_GET_SET(float, offsetSize);
	UAB_BUILD_GET_SET(CColor, Color);
	UAB_BUILD_GET_SET(int, ActiveBilboards);
	CEmptyPointerClass* GetLuaSizeX()
	{
		return (CEmptyPointerClass*)&m_SizeX;
	}
	CEmptyPointerClass* GetLuaSizeY()
	{
		return (CEmptyPointerClass*)&m_SizeY;
	}
	CEmptyPointerClass* GetLuaSizeOffset()
	{
		return (CEmptyPointerClass*)&m_offsetSize;
	}
	CEmptyPointerClass* GetLuaColor()
	{
		return (CEmptyPointerClass*)&m_Color;
	}
	CEmptyPointerClass* GetLuaPosition(int index = 0, int index2 = 0)
	{
		return (CEmptyPointerClass*)&(m_BilboardData[index].Position[index2]);
	}
};

#endif //BILBOARD_SYSTEM_INSTANCE_H