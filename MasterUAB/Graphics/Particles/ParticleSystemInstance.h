#ifndef PARTICLE_SYSTEM_INSTANCE_H
#define PARTICLE_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "Math\Color.h"
#include "RenderableObjects\VertexTypes.h"

#include <random>

class CParticleSystemType;
class CXMLTreeNode;
class CRenderableVertexs;

class CParticleSystemInstance : public CRenderableObject
{
private:
	unsigned short l_IndexType = 16;
	float m_NextParticleEmission;
	bool m_Awake;
	float m_AwakeTimer;
	Vect3f m_EmissionBoxHalfSize;
	float m_EmissionVolume, m_EmissionScaler;	

	struct ParticleData
	{
		Vect3f Position, Velocity, Acceleration;
		int CurrentFrame;
		float TimeToNextFrame;
		float LifeTime, TotalLife;
		float Angle, AngularSpeed, AngularAcceleration;
		CColor Color;
		float Size;
	};

	int m_ActiveParticles;
	ParticleData m_ParticleData[MAX_PARTICLE_PER_INSTANCE];

	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;
	std::random_device rnd;

	MV_POSITION_COLOR_TEXTURE_TEXTURE2_VERTEX *m_ParticleRenderableData[MAX_PARTICLE_PER_INSTANCE];
	CRenderableVertexs *m_RenderableVertex;
public:
	CParticleSystemInstance(){};
	CParticleSystemInstance(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(CParticleSystemType *, Type);
	UAB_GET_PROPERTY(float, NextParticleEmission);
	UAB_GET_PROPERTY(bool, Awake);
	UAB_GET_PROPERTY(float, AwakeTimer);
	UAB_GET_PROPERTY(Vect3f, EmissionBoxHalfSize);
	UAB_GET_PROPERTY(float, EmissionVolume);
	UAB_GET_PROPERTY(float, EmissionScaler);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	/*TRenderableObjectType GetClassType() {
		return PARTICLE_EMITER;
	}*/

	float GetRandomValue(float min, float max);
	Vect3f GetRandomValue(Vect3f min, Vect3f max);
	CColor GetRandomValue(CColor min, CColor max);
	float GetRandomValue(Vect2f value);
	float ComputeTimeToNextParticle();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
};

#endif //PARTICLE_SYSTEM_INSTANCE_H