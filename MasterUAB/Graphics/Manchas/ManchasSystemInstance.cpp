#include "ManchasSystemInstance.h"
#include "ManchasManager.h"
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
#include "LevelManager\LevelManager.h"

<<<<<<< HEAD
CManchasSystemInstance::CManchasSystemInstance(tinyxml2::XMLElement* TreeNode,const std::string &_LevelId) : CRenderableObject(TreeNode,_LevelId), m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
=======
CManchasSystemInstance::CManchasSystemInstance(tinyxml2::XMLElement* TreeNode) : CRenderableObject(TreeNode)
>>>>>>> develop
{
	m_Awake = TreeNode->GetBoolProperty("awake");
	m_ActiveManchas = 0;
	m_Type = UABEngine.GetInstance()->GetLevelManager()->GetResource(_LevelId)->GetManchasManager()->GetResource(TreeNode->GetPszProperty("type"));
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_ManchasRenderableData, MAX_MANCHAS, MAX_MANCHAS, true);
	m_NextParticleEmission = ComputeTimeToNextMancha();
	m_Name = TreeNode->GetPszProperty("name");
}

CManchasSystemInstance::~CManchasSystemInstance(void)
{
	Destroy();
}

void CManchasSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
}

float CManchasSystemInstance::ComputeTimeToNextMancha()
{
	return UABEngine.GetRandomValue(m_Type->GetEmitTime());
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
				manchas.Position.x = UABEngine.GetRandomValue(Vect2f(-1.0f, 1.0f));
				manchas.Position.y = UABEngine.GetRandomValue(Vect2f(-1.0f, 1.0f));
				manchas.Frame = (int)UABEngine.GetRandomValue(0, (float)m_Type->GetFrames());
				manchas.LifeTime = 0;
				manchas.TotalLife = UABEngine.GetRandomValue(m_Type->GetLife());
				manchas.Opacity = 0;
				manchas.MaxOpacity = UABEngine.GetRandomValue(m_Type->GetOpacity());
				manchas.Size = UABEngine.GetRandomValue(m_Type->GetSize());
				manchas.SizeSpeed = UABEngine.GetRandomValue(m_Type->GetSizeSpeed());
				manchas.Color = UABEngine.GetRandomValue(m_Type->GetColor1(), m_Type->GetColor2());
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

void CManchasSystemInstance::Save(FILE* _File, std::string _layer)
{
	fprintf_s(_File, "\t<manchas_instance name=\"%s\" layer=\"%s\" type=\"%s\" awake=\"%s\" visible=\"%s\"/>\n",
		m_Name.c_str(), _layer.c_str(), m_Type->GetName().c_str(), m_Awake ? "true" : "false" , m_Visible ? "true" : "false");
}