#ifndef MANCHAS_SYSTEM_INSTANCE_H
#define MANCHAS_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "Math\Color.h"
#include "RenderableObjects\VertexTypes.h"

#include <random>

class CManchasSystemType;
class CXMLTreeNode;
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
		int Frame;
		CColor Color;
	};

	int m_ActiveManchas;
	ManchaData m_ManchaData[MAX_MANCHAS];

	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;
	std::random_device rnd;

	MV_POSITION4_COLOR_TEXTURE_VERTEX m_ManchasRenderableData[MAX_MANCHAS];
	CRenderableVertexs *m_RenderableVertex;
public:
	CManchasSystemInstance(CXMLTreeNode &TreeNode);
	virtual ~CManchasSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(CManchasSystemType *, Type);
	UAB_BUILD_GET_SET(float, NextParticleEmission);
	UAB_BUILD_GET_SET(bool, Awake);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	/*TRenderableObjectType GetClassType() {
		return PARTICLE_EMITER;
	}*/

	float GetRandomValue(float min, float max);
	Vect3f GetRandomValue(Vect3f min, Vect3f max);
	CColor GetRandomValue(CColor min, CColor max);
	float GetRandomValue(Vect2f value);
	float ComputeTimeToNextMancha();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
	//void Save(FILE* _File, std::string _layer);
	CEmptyPointerClass* GetLuaAwake()
	{
		return (CEmptyPointerClass*)&m_Awake;
	}
};

#endif //MANCHAS_SYSTEM_INSTANCE_H