#ifndef PARTICLE_SYSTEM_INSTANCE_H
#define PARTICLE_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "Math\Color.h"

#include <random>

class CParticleSystemType;
class CXMLTreeNode;

class CParticleSystemInstance : public CRenderableObject
{
private:
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

	/*PARTICLE_VERTEX m_ParticleRenderableData[MAX_PARTICLE_PER_INSTANCE];
	CRendableVertexs *m_Vertices;*/
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
};

#endif //PARTICLE_SYSTEM_INSTANCE_H