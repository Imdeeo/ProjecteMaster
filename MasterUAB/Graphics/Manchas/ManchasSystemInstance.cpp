#include "ManchasSystemInstance.h"
#include "ManchasManager.h"
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
#include "Camera\CameraControllerManager.h"
#include "Math\MathUtils.h"

CManchasSystemInstance::CManchasSystemInstance(CXMLTreeNode &TreeNode) : CRenderableObject(TreeNode), m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
{
	m_Awake = TreeNode.GetBoolProperty("awake");
	m_ActiveManchas = 0;
	m_Type = UABEngine.GetInstance()->GetManchasManager()->GetResource(TreeNode.GetPszProperty("type"));
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_ManchasRenderableData, MAX_MANCHAS, MAX_MANCHAS, true);
	m_NextParticleEmission = ComputeTimeToNextMancha();
	m_Name = TreeNode.GetPszProperty("name");
}

CManchasSystemInstance::~CManchasSystemInstance(void)
{
	Destroy();
}

void CManchasSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
}



float CManchasSystemInstance::GetRandomValue(float min, float max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	float value = mathUtils::Lerp(min, max, a);
	return value;
}

Vect3f CManchasSystemInstance::GetRandomValue(Vect3f min, Vect3f max)
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

float CManchasSystemInstance::GetRandomValue(Vect2f value)
{
	return GetRandomValue(value.x, value.y);
}

float CManchasSystemInstance::ComputeTimeToNextMancha()
{
	return GetRandomValue(m_Type->GetEmitTime());
}

CColor CManchasSystemInstance::GetRandomValue(CColor min, CColor max)
{
	float a = m_UnitDistribution(m_RandomEngine);
	CColor value = min.Lerp(max, a);
	return value;
}

void CManchasSystemInstance::Update(float ElapsedTime)
{
	if (m_Awake)
	{
		m_NextParticleEmission -= ElapsedTime;
		while (m_NextParticleEmission < 0)
		{
			if (m_ActiveManchas < MAX_MANCHAS)
			{
				ManchaData manchas = {};
				manchas.Position.x = GetRandomValue(Vect2f(-1.0f,1.0f));
				manchas.Position.y = GetRandomValue(Vect2f(-1.0f, 1.0f)); 
				manchas.Frame = (int)GetRandomValue(0, (float)m_Type->GetFrames());
				manchas.LifeTime = 0;
				manchas.TotalLife = GetRandomValue(m_Type->GetLife());
				manchas.Opacity = 0;
				manchas.MaxOpacity = GetRandomValue(m_Type->GetOpacity());
				manchas.Size = GetRandomValue(m_Type->GetSize());
				manchas.SizeSpeed = GetRandomValue(m_Type->GetSizeSpeed());
				manchas.Color = GetRandomValue(m_Type->GetColor1(), m_Type->GetColor2());
				m_ManchaData[m_ActiveManchas] = manchas;
				++m_ActiveManchas;
			}

			m_NextParticleEmission += ComputeTimeToNextMancha();
		}
	}

	for (int i = 0; i < m_ActiveManchas; ++i)
	{
		ManchaData *mancha = &m_ManchaData[i];
		mancha->LifeTime += ElapsedTime;
		
		if (m_ManchaData[i].LifeTime > m_ManchaData[i].TotalLife)
		{
			--m_ActiveManchas;
			m_ManchaData[i] = m_ManchaData[m_ActiveManchas];
			--i;
		}
		else
		{
			if (mancha->LifeTime < mancha->TotalLife / 2)
			{
				mancha->Opacity = mancha->MaxOpacity*(mancha->LifeTime / (mancha->TotalLife / 2));
			}
			else
			{
				mancha->Opacity = mancha->MaxOpacity - mancha->MaxOpacity*((mancha->LifeTime - (mancha->TotalLife / 2)) / (mancha->TotalLife / 2));
			}
			mancha->Size += mancha->SizeSpeed*ElapsedTime;
		}
	}	
}

void CManchasSystemInstance::Render(CRenderManager *RM)
{
	if (m_Visible)
	{
		RM->GetContextManager()->SetWorldMatrix(GetTransform());
		CRenderableObject::Render(RM);

		for (int i = 0; i < m_ActiveManchas; ++i)
		{
			ManchaData *mancha = &m_ManchaData[i];

			m_ManchasRenderableData[i].Position = Vect4f(mancha->Position.x, mancha->Position.y, 0.9f, 1.0f);

			m_ManchasRenderableData[i].Color = mancha->Color;
			m_ManchasRenderableData[i].Color.w = mancha->Opacity;

			m_ManchasRenderableData[i].UV.x = mancha->Frame;			
			m_ManchasRenderableData[i].UV.y = mancha->Size;
		}

		if (m_ActiveManchas > 0)
		{
			CMaterial* l_Material = m_Type->GetMaterial();
 			l_Material->Apply();
			CEffectTechnique* l_EffectTechnique = l_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			m_RenderableVertex->UpdateVertexs(m_ManchasRenderableData, MAX_MANCHAS);
			m_RenderableVertex->Render(RM, l_EffectTechnique, CEffectManager::GetRawData(), m_ActiveManchas);
		}
	}
}


/*void CParticleSystemInstance::Save(FILE* _File, std::string _layer)
{
		fprintf_s(_File, "\t<particle_instance name=\"%s\" layer=\"%s\" type=\"%s\" position=\"%f %f %f\" "
			"next_particle_emission=\"%f\" awake=\"%s\" awake_timer=\"%f\" emission_box_half_size=\"%f %f %f\" visible=\"%s\"/>\n",
		m_Name.c_str(), _layer.c_str(), m_Type->GetName().c_str(), m_Position.x, m_Position.y, m_Position.z, 
		m_NextParticleEmission, m_Awake ? "true" : "false", m_AwakeTimer, m_EmissionBoxHalfSize.x, m_EmissionBoxHalfSize.y, m_EmissionBoxHalfSize.z,
		m_Visible ? "true" : "false");
}*/