#include "ParticleSystemInstance.h"
#include "ParticleManager.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Materials\Material.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "Effects\EffectManager.h"

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode &TreeNode) : 
	CRenderableObject(TreeNode), m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
{
	CXMLTreeNode l_Element = TreeNode;
	m_Type = UABEngine.GetInstance()->GetParticleManager()->GetResource(l_Element.GetPszProperty("type"));
	m_NextParticleEmission = l_Element.GetFloatProperty("next_particle_emission", 1.0f);
	m_Awake = l_Element.GetBoolProperty("awake", false);
	m_AwakeTimer = l_Element.GetFloatProperty("awake_timer", 1.0f);
	m_EmissionBoxHalfSize = l_Element.GetVect3fProperty("emission_box_half_size", Vect3f(1.0, 1.0, 1.0)) * 0.5f;
	m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
	m_EmissionScaler = m_Type->GetEmitAbsolute() ? 1 : 1.0f / m_EmissionVolume;
	m_ActiveParticles = 0;
}

CParticleSystemInstance::~CParticleSystemInstance(void)
{
	Destroy();
}

void CParticleSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
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
	value.y = mathUtils::Lerp(min.y, max.y, a1);
	value.z = mathUtils::Lerp(min.z, max.z, a1);
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
				particle.Position = GetRandomValue(-m_EmissionBoxHalfSize, m_EmissionBoxHalfSize);
				particle.Velocity = GetRandomValue(m_Type->GetStartingSpeed1(), m_Type->GetStartingSpeed2());
				particle.Acceleration = GetRandomValue(m_Type->GetStartingAcceleration1(), m_Type->GetStartingAcceleration2());

				particle.CurrentFrame = 0 ;
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
		particle->Position += particle->Velocity * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->Acceleration;
		particle->Velocity += particle->Acceleration * ElapsedTime;
		particle->TimeToNextFrame -= ElapsedTime;
		particle->LifeTime += ElapsedTime;

		m_ParticleRenderableData[i] = (MV_POSITION_COLOR_TEXTURE_TEXTURE2_VERTEX*)malloc(sizeof(MV_POSITION_COLOR_TEXTURE_TEXTURE2_VERTEX));		

		while (particle->TimeToNextFrame < 0 && (m_Type->GetLoopFrames() || particle->CurrentFrame < m_Type->GetNumFrames() - 1))
		{
			particle->CurrentFrame - (particle->CurrentFrame + 1) % m_Type->GetNumFrames();
			particle->TimeToNextFrame += m_Type->GetTimePerFrame();
		}

		if (m_ParticleData[i].LifeTime > m_ParticleData[i].TotalLife)
		{
			--m_ActiveParticles;
			m_ParticleData[i] = m_ParticleData[m_ActiveParticles];
			--i;
		}
	}

	m_RenderableVertex = new CUABLinesListRenderableVertexs<MV_POSITION_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_ParticleRenderableData[0], 1, 1, true);
}

void CParticleSystemInstance::Render(CRenderManager *RM)
{
	CRenderableObject::Render(RM);
	RM->GetContextManager()->SetWorldMatrix(GetTransform());
	CMaterial* l_Material = m_Type->GetMaterial();
	l_Material->Apply();
	CEffectTechnique* l_EffectTechnique = l_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
	CEffectManager::SetSceneConstants(l_EffectTechnique);
	m_RenderableVertex->Render(RM, l_EffectTechnique, CEffectManager::GetRawData());
}

//void CParticleSystemInstance::RenderDebug(CRenderManager *RM)
//{
//
//}