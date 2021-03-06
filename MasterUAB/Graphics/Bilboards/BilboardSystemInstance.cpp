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

CBilboardSystemInstance::CBilboardSystemInstance(CLevel *_Level) :CRenderableObject(_Level)
{}

CBilboardSystemInstance::CBilboardSystemInstance(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :
	CRenderableObject(TreeNode,_Level)
{
	m_Type = _Level->GetBilboardManager()->GetResource(TreeNode->GetPszProperty("type"));
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
	m_SizeX = TreeNode->GetFloatProperty("sizeX", 0.1);
	m_SizeY = TreeNode->GetFloatProperty("sizeY", 0.1);
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

		for (size_t i = 0; i < m_ActiveBilboards; ++i)
		{
			BilboardData *Bilboard = &m_BilboardData[i];

			m_BilboardRenderableData[i].Position = Bilboard->Position;

			m_BilboardRenderableData[i].Color = m_Color;

			m_BilboardRenderableData[i].UV.x = UABEngine.GetRandomValue(m_SizeX + m_offsetSize, m_SizeX - m_offsetSize);
			m_BilboardRenderableData[i].UV.y = UABEngine.GetRandomValue(m_SizeY + m_offsetSize, m_SizeY - m_offsetSize);
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

void CBilboardSystemInstance::Save(FILE* _File, std::string _layer)
{
	fprintf_s(_File, "\t<bilboard_instance name=\"%s\" layer=\"%s\" type=\"%s\" start=\"%s\" sizeX=\"%f\" sizeY=\"%f\" offsetSize=\"%f\" color=\"%f %f %f %f\" visible=\"%s\">\n",
		m_Name.c_str(), _layer.c_str(), m_Type->GetName().c_str(), m_Start ? "true" : "false", m_SizeX, m_SizeY, m_offsetSize, m_Color.x, m_Color.y, m_Color.z, m_Color.w,
		m_Visible ? "true" : "false");
	for (size_t i = 0; i < m_ActiveBilboards; ++i)
	{
		fprintf(_File, "\t\t<position value=\"%f %f %f\"/>\n", m_BilboardData[i].Position.x, m_BilboardData[i].Position.y, m_BilboardData[i].Position.z);
	}
	fprintf_s(_File, "\t</ bilboard_instance>\n");
}