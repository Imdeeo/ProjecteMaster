#ifndef MANCHAS_SYSTEM_INSTANCE_H
#define MANCHAS_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "Math\Color.h"
#include "RenderableObjects\VertexTypes.h"
#include "XML\tinyxml2.h"

class CManchasSystemType;
class CRenderableVertexs;

class CManchasSystemInstance : public CRenderableObject
{
private:
	unsigned short l_IndexType = 16;

	struct ManchaData
	{
		Vect2f Position;
		float LifeTime, TotalLife;
		float Size;
		float SizeSpeed;
		float Opacity, MaxOpacity;
		float Frame;
		CColor Color;
	};

	int m_ActiveManchas;
	ManchaData m_ManchaData[MAX_MANCHAS];

	MV_POSITION4_COLOR_TEXTURE_VERTEX m_ManchasRenderableData[MAX_MANCHAS];
	CRenderableVertexs *m_RenderableVertex;
public:
	CManchasSystemInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CManchasSystemInstance(void);
	void Destroy();
	std::string GetTipo(){ return "ManchasInstance"; };
	UAB_BUILD_GET_SET(CManchasSystemType *, Type);
	UAB_BUILD_GET_SET(float, NextParticleEmission);
	UAB_BUILD_GET_SET(bool, Awake);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	/*TRenderableObjectType GetClassType() {
		return PARTICLE_EMITER;
	}*/

	float ComputeTimeToNextMancha();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
	void Save(FILE* _File, std::string _layer);

	CEmptyPointerClass* GetLuaAwake()
	{
		return (CEmptyPointerClass*)&m_Awake;
	}
};

#endif //MANCHAS_SYSTEM_INSTANCE_H