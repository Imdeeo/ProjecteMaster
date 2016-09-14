#include "LineRenderer.h"
#include "Materials/Material.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "D3D11.h"
#include "XML\tinyxml2.h"
#include "Effects/EffectManager.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "Materials/MaterialManager.h"
#include "DebugRender.h"

CLineRenderer::CLineRenderer(tinyxml2::XMLElement* TreeNode)
	: CRenderableObject(TreeNode)
	, m_Size(1.0f)
{
	ZeroMemory(&m_LineRenderableData, MAX_LINE_LENGHT*sizeof(MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX));
	/*for (unsigned int i = 0; i < MAX_LINE_LENGHT; ++i )
	{
		m_LineRenderableData[i].Position = Vect3f(float(i), 1.0f, 0.0f);
		m_LineRenderableData[i].Color = CColor(1.0f, 0.0f, 0.0f, 1.0f);
	}*/
	m_LineRenderableData[0].Position = Vect4f(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineRenderableData[0].UV = Vect2f(4.0f, 0.0f);
	m_LineRenderableData[0].Color = CColor(1.0f, 0.0f, 1.0f, 1.0f);
	//m_LineRenderableData[1].Position = Vect3f(1.0f, 2.0f, 0.0f);
	//m_LineRenderableData[1].Color = CColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_LinesCount = 1;
	m_RenderableVertex = new CUABLinesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_LineRenderableData, MAX_LINE_LENGHT, MAX_LINE_LENGHT, true);
	m_Material = UABEngine.GetMaterialManager()->GetResource("Base");
}

CLineRenderer::~CLineRenderer()
{
	Destroy();
}

void CLineRenderer::Destroy()
{
}

CRenderableObjectTechnique* CLineRenderer::GetRenderableObjectTechnique()
{
	return m_RenderableObjectTechnique;
}

void CLineRenderer::Save(FILE* _File)
{
	// TODO
	/*
	fprintf_s(_File, "\t<material name=\"%s\" renderable_object_technique=\"%s\">\n",m_Name.c_str(),m_RenderableObjectTechnique->GetName().c_str());

	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		if (m_Textures[i]!=nullptr)
		{
			m_Textures[i]->Save(_File, 2);
		}
	}
	for (size_t i = 0; i < m_Parameters.size(); i++)
	{
		m_Parameters[i]->Save(_File, 2);
	}

	fprintf_s(_File, "\t</material>\n");*/
}

void CLineRenderer::Update(float ElapsedTime)
{
	
}

void CLineRenderer::Render(CRenderManager *RM)
{
	CRenderableObject::Render(RM);
	RM->GetContextManager()->SetWorldMatrix(GetTransform());
	m_Material->Apply();
	CEffectTechnique* l_ET = m_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
	CEffectManager::SetSceneConstants(l_ET);
	

	/*
	CEffectTechnique * l_ET = RM->GetDebugRender()->GetDebugTechnique();
	CEffectManager::SetSceneConstants(l_ET);*/
	m_RenderableVertex->UpdateVertexs(m_LineRenderableData, MAX_PARTICLE_PER_INSTANCE);
	m_RenderableVertex->Render(RM, l_ET, CEffectManager::GetRawData(), m_LinesCount);
}