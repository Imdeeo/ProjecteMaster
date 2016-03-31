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
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_ParticleRenderableData, MAX_PARTICLE_PER_INSTANCE, MAX_PARTICLE_PER_INSTANCE, true);
	//m_RenderableVertex = new CUABTrianglesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_ParticleRenderableData, MAX_PARTICLE_PER_INSTANCE, MAX_PARTICLE_PER_INSTANCE, true);
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
				particle.Position = GetRandomValue(-m_EmissionBoxHalfSize, m_EmissionBoxHalfSize);
				particle.Velocity = GetRandomValue(m_Type->GetStartingSpeed1(), m_Type->GetStartingSpeed2());
				particle.Acceleration = GetRandomValue(m_Type->GetStartingAcceleration1(), m_Type->GetStartingAcceleration2());
				particle.AngularSpeed = GetRandomValue(m_Type->GetStartingAngularSpeed()[0], m_Type->GetStartingAngularSpeed()[1]);
				particle.AngularAcceleration = GetRandomValue(m_Type->GetAngularAcceleration()[0], m_Type->GetAngularAcceleration()[1]);

				//particle.Size = GetRandomValue(m_Type->GetSize().x, m_Type->GetSize().y);
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
		particle->Position += particle->Velocity * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->Acceleration;
		particle->Velocity += particle->Acceleration * ElapsedTime;
		particle->Angle += particle->AngularSpeed * ElapsedTime + 0.5f * ElapsedTime * ElapsedTime * particle->AngularAcceleration;
		particle->TimeToNextFrame -= ElapsedTime;
		particle->LifeTime += ElapsedTime;			

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

			if (particle->SizeControlPoint + 1 < m_Type->m_ControlPointSizes.size())
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

			if (particle->ColorControlPoint + 1 < m_Type->m_ControlPointColors.size())
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

		if (m_ParticleData[i].LifeTime > m_ParticleData[i].TotalLife)
		{
			--m_ActiveParticles;
			m_ParticleData[i] = m_ParticleData[m_ActiveParticles];
			--i;
		}
	}
}

void CParticleSystemInstance::Render(CRenderManager *RM)
{
	RM->GetContextManager()->SetWorldMatrix(GetTransform());
	CRenderableObject::Render(RM);

	for (int i = 0; i < m_ActiveParticles; ++i)
	{
		ParticleData *particle = &m_ParticleData[i];

		m_ParticleRenderableData[i].Position = particle->Position;

		CColor ColorControlAlpha = particle->LastColor.interpolate(particle->LastColor, particle->NextColor, 0.5f);
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
	
	/*m_ParticleRenderableData[0].Position = Vect3f(0, 0, 0);
	m_ParticleRenderableData[0].UV = Vect2f(1, 0.5);
	m_ParticleRenderableData[1].Position = Vect3f(1, 0, 0);
	m_ParticleRenderableData[2].Position = Vect3f(0, 1, 0);

	CMaterial* l_Material = m_Type->GetMaterial();
	l_Material->Apply();
	CEffectTechnique* l_EffectTechnique = l_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
	CEffectManager::SetSceneConstants(l_EffectTechnique);
	m_RenderableVertex->UpdateVertexs(m_ParticleRenderableData, MAX_PARTICLE_PER_INSTANCE);
	m_RenderableVertex->Render(RM, l_EffectTechnique, CEffectManager::GetRawData(), 3);*/
}