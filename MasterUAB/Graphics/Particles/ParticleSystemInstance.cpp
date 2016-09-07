#include "ParticleSystemInstance.h"
#include "ParticleManager.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Materials\Material.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "Effects\EffectManager.h"
#include "Camera\CameraControllerManager.h"
#include "Math\MathUtils.h"

CParticleSystemInstance::CParticleSystemInstance(tinyxml2::XMLElement* TreeNode) :
	CRenderableObject(TreeNode), m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
{
	m_Type = UABEngine.GetInstance()->GetParticleManager()->GetResource(TreeNode->GetPszProperty("type"));
	m_NextParticleEmission = TreeNode->GetFloatProperty("next_particle_emission", 1.0f);
	m_Awake = TreeNode->GetBoolProperty("awake", false);
	m_AwakeTimer = TreeNode->GetFloatProperty("awake_timer", 1.0f);
	m_EmissionBoxHalfSize = TreeNode->GetVect3fProperty("emission_box_half_size", Vect3f(1.0, 1.0, 1.0));
	m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
	m_EmissionScaler = m_Type->GetEmitAbsolute() ? 1 : 1.0f / m_EmissionVolume;
	m_ActiveParticles = 0;
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_ParticleRenderableData, MAX_PARTICLE_PER_INSTANCE, MAX_PARTICLE_PER_INSTANCE, true);
}

CParticleSystemInstance::~CParticleSystemInstance(void)
{
	Destroy();
}

void CParticleSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
}

float CParticleSystemInstance::GetDistanceToCamera(ParticleData *particle)
{
	CCameraController* aux = UABEngine.GetCameraControllerManager()->GetMainCamera();
	
	Vect3f P = particle->Position - aux->GetPosition();

	return P * aux->GetForward();
}

float CParticleSystemInstance::GetRandomValue(float min, float max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	float value = mathUtils::Lerp(min, max, a);
	return value;
}

Vect3f CParticleSystemInstance::GetRandomValue(Vect3f min, Vect3f max)
{
	float a1 = m_UnitDistribution(m_RandomEngine);
	float a2 = m_UnitDistribution(m_RandomEngine);
	float a3 = m_UnitDistribution(m_RandomEngine);
	Vect3f value;
	value.x = mathUtils::Lerp(min.x, max.x, a1);
	value.y = mathUtils::Lerp(min.y, max.y, a2);
	value.z = mathUtils::Lerp(min.z, max.z, a3);
	return value;
}

CColor CParticleSystemInstance::GetRandomValue(CColor min, CColor max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	CColor value = min.Lerp(max, a);
	return value;
}

float CParticleSystemInstance::GetRandomValue(Vect2f value)
{
	return GetRandomValue(value.x, value.y);
}

float CParticleSystemInstance::ComputeTimeToNextParticle()
{
	float particlePerSecPerM3 = GetRandomValue(m_Type->GetEmitRate());
	return m_EmissionScaler / particlePerSecPerM3;
}

void CParticleSystemInstance::Update(float ElapsedTime)
{
	m_AwakeTimer -= ElapsedTime;
	while (m_AwakeTimer < 0)
	{
		m_Awake = !m_Awake;
		m_AwakeTimer += GetRandomValue(m_Awake ? m_Type->GetAwakeTime() : m_Type->GetSleepTime());
	}

	if (m_Awake)
	{
		m_NextParticleEmission -= ElapsedTime;
		while (m_NextParticleEmission < 0)
		{
			if (m_ActiveParticles < MAX_PARTICLE_PER_INSTANCE)
			{
				ParticleData particle = {};
				particle.Position = GetRandomValue(-m_EmissionBoxHalfSize, m_EmissionBoxHalfSize)+m_Position;
				particle.AngularSpeed = GetRandomValue(m_Type->GetStartingAngularSpeed()[0], m_Type->GetStartingAngularSpeed()[1]);
				particle.AngularAcceleration = GetRandomValue(m_Type->GetAngularAcceleration()[0], m_Type->GetAngularAcceleration()[1]);

				particle.SizeControlPoint = 0;
				particle.LastSizeControlTime = 0;
				particle.LastSize = GetRandomValue(m_Type->m_ControlPointSizes[0].m_Size);
				particle.NextSizeControlTime = m_Type->m_ControlPointSizes.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Time);
				particle.NextSize = m_Type->m_ControlPointSizes.size() < 2 ? particle.LastSize : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Size);

				particle.ColorControlPoint = 0;
				particle.LastColorControlTime = 0;
				particle.LastColor = GetRandomValue(m_Type->m_ControlPointColors[0].m_Color1, m_Type->m_ControlPointColors[0].m_Color2);
				particle.NextColorControlTime = m_Type->m_ControlPointColors.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointColors[1].m_Time);
				particle.NextColor = m_Type->m_ControlPointColors.size() < 2 ? particle.LastColor : GetRandomValue(m_Type->m_ControlPointColors[1].m_Color1, m_Type->m_ControlPointColors[1].m_Color2);
				
				particle.SpeedControlPoint = 0;
				particle.LastSpeedControlTime = 0;
				particle.LastSpeed = GetRandomValue(m_Type->m_ControlPointSpeeds[0].m_Speed1, m_Type->m_ControlPointSpeeds[0].m_Speed2);
				particle.NextSpeedControlTime = m_Type->m_ControlPointSpeeds.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointSpeeds[1].m_Time);
				particle.NextSpeed = m_Type->m_ControlPointSpeeds.size() < 2 ? particle.LastSpeed : GetRandomValue(m_Type->m_ControlPointSpeeds[1].m_Speed1, m_Type->m_ControlPointSpeeds[1].m_Speed2);

				particle.AccelerationControlPoint = 0;
				particle.LastAccelerationControlTime = 0;
				particle.LastAcceleration = GetRandomValue(m_Type->m_ControlPointAccelerations[0].m_Acceleration1, m_Type->m_ControlPointAccelerations[0].m_Acceleration2);
				particle.NextAccelerationControlTime = m_Type->m_ControlPointAccelerations.size() < 2 ? particle.TotalLife : GetRandomValue(m_Type->m_ControlPointAccelerations[1].m_Time);
				particle.NextAcceleration = m_Type->m_ControlPointAccelerations.size() < 2 ? particle.LastAcceleration : GetRandomValue(m_Type->m_ControlPointAccelerations[1].m_Acceleration1, m_Type->m_ControlPointAccelerations[1].m_Acceleration2);

				particle.Angle = GetRandomValue(m_Type->GetStartingAngle().x, m_Type->GetStartingAngle().y);

				particle.CurrentFrame = 0;
				particle.TimeToNextFrame = m_Type->GetTimePerFrame();

				particle.LifeTime = 0;
				particle.TotalLife = GetRandomValue(m_Type->GetLife());

				m_ParticleData[m_ActiveParticles] = particle;
				++m_ActiveParticles;
			}

			m_NextParticleEmission += ComputeTimeToNextParticle();
		}
	}

	for (int i = 0; i < m_ActiveParticles; ++i)
	{
		ParticleData *particle = &m_ParticleData[i];

		float SpeedControlAlpha = (particle->LifeTime < particle->NextSpeedControlTime) ?
			(particle->LifeTime - particle->LastSpeedControlTime) / (particle->NextSpeedControlTime - particle->LastSpeedControlTime) :
			1.0f;

		float AccelerationControlAlpha = (particle->LifeTime < particle->NextAccelerationControlTime) ?
			(particle->LifeTime - particle->LastAccelerationControlTime) / (particle->NextAccelerationControlTime - particle->LastAccelerationControlTime) :
			1.0f;

		particle->Velocity = particle->LastSpeed.Lerp(particle->NextSpeed, SpeedControlAlpha);
		particle->Acceleration = particle->LastAcceleration.Lerp(particle->NextAcceleration, AccelerationControlAlpha);
		particle->Position += particle->Velocity * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->Acceleration;
		//particle->Velocity += particle->Acceleration * ElapsedTime;
		particle->Angle += particle->AngularSpeed * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->AngularAcceleration;
		particle->TimeToNextFrame -= ElapsedTime;
		particle->LifeTime += ElapsedTime;		
		particle->DistanceToCamera = GetDistanceToCamera(particle);

		while (particle->TimeToNextFrame < 0 && (m_Type->GetLoopFrames() || particle->CurrentFrame < m_Type->GetNumFrames() - 1))
		{
			particle->CurrentFrame = (particle->CurrentFrame + 1) % m_Type->GetNumFrames();
			particle->TimeToNextFrame += m_Type->GetTimePerFrame();
		}

		while (particle->LifeTime > particle->NextSizeControlTime && particle->LifeTime < particle->TotalLife)
		{
			++particle->SizeControlPoint;
			
			particle->LastSize = particle->NextSize;
			particle->LastSizeControlTime = particle->NextSizeControlTime;

			if (particle->SizeControlPoint + 1 < (int)m_Type->m_ControlPointSizes.size())
			{
				particle->NextSize = GetRandomValue(m_Type->m_ControlPointSizes[particle->SizeControlPoint + 1].m_Size);
				particle->NextSizeControlTime = GetRandomValue(m_Type->m_ControlPointSizes[particle->SizeControlPoint + 1].m_Time);
			}
			else
			{
				particle->NextSizeControlTime = particle->TotalLife;
			}
		}

		while (particle->LifeTime > particle->NextColorControlTime && particle->LifeTime < particle->TotalLife)
		{
			++particle->ColorControlPoint;

			particle->LastColor = particle->NextColor;
			particle->LastColorControlTime = particle->NextColorControlTime;

			if (particle->ColorControlPoint + 1 < (int)m_Type->m_ControlPointColors.size())
			{
				particle->NextColor = GetRandomValue(m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Color1, 
					m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Color2);
				particle->NextColorControlTime = GetRandomValue(m_Type->m_ControlPointColors[particle->ColorControlPoint + 1].m_Time);
			}
			else
			{
				particle->NextColorControlTime = particle->TotalLife;
			}
		}

		while (particle->LifeTime > particle->NextSpeedControlTime && particle->LifeTime < particle->TotalLife)
		{
			++particle->SpeedControlPoint;

			particle->LastSpeed = particle->NextSpeed;
			particle->LastSpeedControlTime = particle->NextSpeedControlTime;

			if (particle->SpeedControlPoint + 1 < (int)m_Type->m_ControlPointSpeeds.size())
			{
				particle->NextSpeed = GetRandomValue(m_Type->m_ControlPointSpeeds[particle->SpeedControlPoint + 1].m_Speed1,
					m_Type->m_ControlPointSpeeds[particle->SpeedControlPoint + 1].m_Speed2);
				particle->NextSpeedControlTime = GetRandomValue(m_Type->m_ControlPointSpeeds[particle->SpeedControlPoint + 1].m_Time);
			}
			else
			{
				particle->NextSpeedControlTime = particle->TotalLife;
			}
		}

		while (particle->LifeTime > particle->NextAccelerationControlTime && particle->LifeTime < particle->TotalLife)
		{
			++particle->AccelerationControlPoint;

			particle->LastAcceleration = particle->NextAcceleration;
			particle->LastAccelerationControlTime = particle->NextAccelerationControlTime;

			if (particle->AccelerationControlPoint + 1 < (int)m_Type->m_ControlPointAccelerations.size())
			{
				particle->NextAcceleration = GetRandomValue(m_Type->m_ControlPointAccelerations[particle->AccelerationControlPoint + 1].m_Acceleration1,
					m_Type->m_ControlPointAccelerations[particle->AccelerationControlPoint + 1].m_Acceleration2);
				particle->NextAccelerationControlTime = GetRandomValue(m_Type->m_ControlPointAccelerations[particle->AccelerationControlPoint + 1].m_Time);
			}
			else
			{
				particle->NextAccelerationControlTime = particle->TotalLife;
			}
		}

		if (m_ParticleData[i].LifeTime > m_ParticleData[i].TotalLife)
		{
			--m_ActiveParticles;
			m_ParticleData[i] = m_ParticleData[m_ActiveParticles];
			--i;
		}		
	}

	if (m_ActiveParticles > 1)
		InsertSort(m_ParticleData, m_ActiveParticles);
}

void CParticleSystemInstance::Render(CRenderManager *RM)
{
	if (m_Visible)
	{
		RM->GetContextManager()->SetWorldMatrix(GetTransform());
		CRenderableObject::Render(RM);

		for (int i = 0; i < m_ActiveParticles; ++i)
		{
			ParticleData *particle = &m_ParticleData[i];

			m_ParticleRenderableData[i].Position = particle->Position;

			CColor ColorControlAlpha = particle->LastColor.interpolate(particle->LastColor, particle->NextColor, (particle->LifeTime-particle->LastColorControlTime)/(particle->NextColorControlTime-particle->LastColorControlTime));
				(particle->LastColor, particle->NextColor, 0.5f);			
			m_ParticleRenderableData[i].Color = ColorControlAlpha;
			//m_ParticleRenderableData[i].Color = particle->LastColor.Lerp(particle->NextColor, ColorControlAlpha);
		
			float SizeControlAlpha = (particle->LifeTime < particle->NextSizeControlTime) ?
				(particle->LifeTime - particle->LastSizeControlTime) / (particle->NextSizeControlTime - particle->LastSizeControlTime) :
				1.0f;

			m_ParticleRenderableData[i].UV.x = mathUtils::Lerp<float>(particle->LastSize, particle->NextSize, SizeControlAlpha);
			m_ParticleRenderableData[i].UV.y = particle->Angle;
			m_ParticleRenderableData[i].UV2.x = (float)particle->CurrentFrame;
			m_ParticleRenderableData[i].UV2.y = (float)m_Type->GetNumFrames();
		}

		if (m_ActiveParticles > 0)
		{
			CMaterial* l_Material = m_Type->GetMaterial();
 			l_Material->Apply();
			CEffectTechnique* l_EffectTechnique = l_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			m_RenderableVertex->UpdateVertexs(m_ParticleRenderableData, MAX_PARTICLE_PER_INSTANCE);
			m_RenderableVertex->Render(RM, l_EffectTechnique, CEffectManager::GetRawData(), m_ActiveParticles);
		}
	}
}

void CParticleSystemInstance::InsertSort(ParticleData arr[], int length) {
	int i, j;
	ParticleData tmp;
	for (i = 1; i < length; ++i) {
		j = i;
		while (j > 0 && arr[j - 1].DistanceToCamera < arr[j].DistanceToCamera) {
			tmp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = tmp;
			--j;
		}
	}
}

void CParticleSystemInstance::Save(FILE* _File, std::string _layer)
{
		fprintf_s(_File, "\t<particle_instance name=\"%s\" layer=\"%s\" type=\"%s\" position=\"%f %f %f\" "
			"next_particle_emission=\"%f\" awake=\"%s\" awake_timer=\"%f\" emission_box_half_size=\"%f %f %f\" visible=\"%s\"/>\n",
		m_Name.c_str(), _layer.c_str(), m_Type->GetName().c_str(), m_Position.x, m_Position.y, m_Position.z, 
		m_NextParticleEmission, m_Awake ? "true" : "false", m_AwakeTimer, m_EmissionBoxHalfSize.x, m_EmissionBoxHalfSize.y, m_EmissionBoxHalfSize.z,
		m_Visible ? "true" : "false");
}