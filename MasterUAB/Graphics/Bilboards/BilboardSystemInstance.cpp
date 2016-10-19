#include "BilboardSystemInstance.h"
#include "BilboardManager.h"
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Materials\Material.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "Effects\EffectManager.h"
#include "Math\MathUtils.h"
#include "LevelManager\LevelManager.h"


CBilboardSystemInstance::CBilboardSystemInstance(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :
	CRenderableObject(TreeNode,_LevelId)
{
	m_Type = UABEngine.GetLevelManager()->GetResource(_LevelId)->GetBilboardManager()->GetResource(TreeNode->GetPszProperty("type"));
	m_ActiveBilboards = 0;
	tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement();
	while (l_Element != nullptr)
	{
		m_BilboardData[m_ActiveBilboards].CurrentFrame = UABEngine.GetRandomValue(0, m_Type->GetNumFrames());
		m_BilboardData[m_ActiveBilboards].Position = l_Element->GetVect3fProperty("value",Vect3f(0,0,0));
		m_BilboardData[m_ActiveBilboards].TimeToNextFrame = UABEngine.GetRandomValue(0, m_Type->GetTimePerFrame());
		m_ActiveBilboards++;
		l_Element = l_Element->NextSiblingElement();
	}
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_BilboardRenderableData, MAX_BILBOARDS_PER_INSTANCE, MAX_BILBOARDS_PER_INSTANCE, true);
	m_Start = TreeNode->GetBoolProperty("start", true);
	m_Size = TreeNode->GetFloatProperty("size", 0.1);
	m_offsetSize = TreeNode->GetFloatProperty("offsetSize", 0.01);
	m_Color = CColor(TreeNode->GetVect4fProperty("color", Vect4f(1,1,1,1)));
}

CBilboardSystemInstance::~CBilboardSystemInstance(void)
{
	Destroy();
}

void CBilboardSystemInstance::Destroy()
{
	//CHECKED_RELEASE(m_Type);
}

void CBilboardSystemInstance::Update(float ElapsedTime)
{
	if (m_Start)
	{
		for (int i = 0; i < m_ActiveBilboards; ++i)
		{
			BilboardData *Bilboard = &m_BilboardData[i];

			//Bilboard->Velocity += Bilboard->Acceleration * ElapsedTime;
			Bilboard->TimeToNextFrame -= ElapsedTime;

			if (Bilboard->TimeToNextFrame < 0)
			{
				Bilboard->CurrentFrame = (Bilboard->CurrentFrame + 1) % m_Type->GetNumFrames();
				Bilboard->TimeToNextFrame += m_Type->GetTimePerFrame();
			}			
		}
	}
}

void CBilboardSystemInstance::Render(CRenderManager *RM)
{
	if (m_Visible)
	{
		RM->GetContextManager()->SetWorldMatrix(GetTransform());
		CRenderableObject::Render(RM);

		for (int i = 0; i < m_ActiveBilboards; ++i)
		{
			BilboardData *Bilboard = &m_BilboardData[i];

			m_BilboardRenderableData[i].Position = Bilboard->Position;

			m_BilboardRenderableData[i].Color = m_Color;

			m_BilboardRenderableData[i].UV.x = UABEngine.GetRandomValue(m_Size + m_offsetSize, m_Size - m_offsetSize);
			m_BilboardRenderableData[i].UV2.x = (float)Bilboard->CurrentFrame;
			m_BilboardRenderableData[i].UV2.y = (float)m_Type->GetNumFrames();
		}

		if (m_ActiveBilboards > 0)
		{
			CMaterial* l_Material = m_Type->GetMaterial();
 			l_Material->Apply();
			CEffectTechnique* l_EffectTechnique = l_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
			CEffectManager::SetSceneConstants(l_EffectTechnique);
			m_RenderableVertex->UpdateVertexs(m_BilboardRenderableData, MAX_BILBOARDS_PER_INSTANCE);
			m_RenderableVertex->Render(RM, l_EffectTechnique, CEffectManager::GetRawData(), m_ActiveBilboards);
		}
	}
}
