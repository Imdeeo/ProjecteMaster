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

	struct ParticleData
	{
		Vect3f Position, Velocity, Acceleration;
		int CurrentFrame;
		float TimeToNextFrame;
		float LifeTime, TotalLife;
		float Angle, AngularSpeed, AngularAcceleration;
		float DistanceToCamera;

		int ColorControlPoint, SizeControlPoint;

		float LastColorControlTime, NextColorControlTime;
		float LastSizeControlTime, NextSizeControlTime;

		CColor LastColor, NextColor;
		float LastSize, NextSize;
	};

	int m_ActiveParticles;
	ParticleData m_ParticleData[MAX_PARTICLE_PER_INSTANCE];

	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;
	std::random_device rnd;

	MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX m_ParticleRenderableData[MAX_PARTICLE_PER_INSTANCE];
	CRenderableVertexs *m_RenderableVertex;
public:
	CParticleSystemInstance(){};
	CParticleSystemInstance(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(CParticleSystemType *, Type);
	UAB_BUILD_GET_SET(float, NextParticleEmission);
	UAB_BUILD_GET_SET(bool, Awake);
	UAB_BUILD_GET_SET(float, AwakeTimer);
	UAB_BUILD_GET_SET(Vect3f, EmissionBoxHalfSize);
	UAB_BUILD_GET_SET(float, EmissionVolume);
	UAB_BUILD_GET_SET(float, EmissionScaler);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	/*TRenderableObjectType GetClassType() {
		return PARTICLE_EMITER;
	}*/

	float GetDistanceToCamera(ParticleData *particle);
	float GetRandomValue(float min, float max);
	Vect3f GetRandomValue(Vect3f min, Vect3f max);
	CColor GetRandomValue(CColor min, CColor max);
	float GetRandomValue(Vect2f value);
	float ComputeTimeToNextParticle();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
	void InsertSort(ParticleData arr[], int length);
	void Save(FILE* _File, std::string _layer);
	
	CEmptyPointerClass* GetLuaEmissionBoxPosition(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Position[index];
	}

	CEmptyPointerClass* GetLuaEmissionBoxHalfSize(int index = 0)
	{
		return (CEmptyPointerClass*)&m_EmissionBoxHalfSize[index];
	}
};

#endif //PARTICLE_SYSTEM_INSTANCE_H