#ifndef PARTICLE_SYSTEM_INSTANCE_H
#define PARTICLE_SYSTEM_INSTANCE_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils.h"
#include "Math\Color.h"
#include "RenderableObjects\VertexTypes.h"
#include "XML\tinyxml2.h"

#include <random>

class CParticleSystemType;
class CRenderableVertexs;
class CFrustum;

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

		int ColorControlPoint, SizeControlPoint, SpeedControlPoint, AccelerationControlPoint;

		float LastColorControlTime, NextColorControlTime;
		float LastSizeControlTime, NextSizeControlTime;
		float LastSpeedControlTime, NextSpeedControlTime;
		float LastAccelerationControlTime, NextAccelerationControlTime;

		CColor LastColor, NextColor;
		float LastSize, NextSize;
		Vect3f LastSpeed, LastAcceleration, NextSpeed, NextAcceleration;
	};

	ParticleData m_ParticleData[MAX_PARTICLE_PER_INSTANCE];

	MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX m_ParticleRenderableData[MAX_PARTICLE_PER_INSTANCE];
	CRenderableVertexs *m_RenderableVertex;

	CFrustum *m_Frustum;
public:
	CParticleSystemInstance(){};
	CParticleSystemInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CParticleSystemInstance(void);
	void Destroy();

	UAB_BUILD_GET_SET(int, ActiveParticles)
	UAB_BUILD_GET_SET(CParticleSystemType *, Type);
	UAB_BUILD_GET_SET(float, NextParticleEmission);
	UAB_BUILD_GET_SET(bool, Start);
	UAB_BUILD_GET_SET(bool, Awake);
	UAB_BUILD_GET_SET(float, AwakeTimer);
	UAB_BUILD_GET_SET(Vect3f, EmissionBoxHalfSize);
	UAB_BUILD_GET_SET(float, EmissionVolume);
	UAB_BUILD_GET_SET(float, EmissionScaler);
	UAB_BUILD_GET_SET(bool, EmissionBoxLimit);

	TRenderableObjectType GetClassType() { return PARTICLE_EMITER; }
	
	/*TRenderableObjectType GetClassType() {
		return PARTICLE_EMITER;
	}*/

	float GetDistanceToCamera(ParticleData *particle);
	float ComputeTimeToNextParticle();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	//void RenderDebug(CRenderManager *RM);
	const bool GetInsideFrustum();
	bool CParticleSystemInstance::IsIntoLimit(Vect3f _position);
	void InsertSort(ParticleData arr[], int length);
	void Save(FILE* _File, std::string _layer);
	std::string GetTipo(){ return "ParticleInstance"; };

	CEmptyPointerClass* GetLuaEmissionBoxPosition(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Position[index];
	}

	CEmptyPointerClass* GetLuaEmissionBoxHalfSize(int index = 0)
	{
		return (CEmptyPointerClass*)&m_EmissionBoxHalfSize[index];
	}

	CEmptyPointerClass* GetLuaEmissionBoxLimit()
	{
		return (CEmptyPointerClass*)&m_EmissionBoxLimit;
	}
};

#endif //PARTICLE_SYSTEM_INSTANCE_H