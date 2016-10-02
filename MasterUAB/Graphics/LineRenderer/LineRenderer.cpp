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

	m_PosInicial = TreeNode->GetVect3fProperty("posInicial", Vect3f(0, 0, 0));
	m_PosFinal = TreeNode->GetVect3fProperty("posFinal", Vect3f(0, 0, 0));
	m_NumPuntos = TreeNode->GetIntProperty("numPuntos", 1);
	m_OffsetX = TreeNode->GetFloatProperty("offsetX", 0.2);
	m_OffsetY = TreeNode->GetFloatProperty("offsetY", 0.2);
	m_Color = CColor(TreeNode->GetVect4fProperty("color", Vect4f(1,1,1,1)));
	m_Size = TreeNode->GetFloatProperty("size", 0.03);
	m_SizeOffset = TreeNode->GetFloatProperty("size_ofset", 0.005);
	m_LinesCount = 1;
	m_RenderableVertex = new CUABPointsListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_TEXTURE2_VERTEX>(m_LineRenderableData, MAX_LINE_LENGHT, MAX_LINE_LENGHT, true);
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
	Vect3f l_Dir = m_PosFinal - m_PosInicial;
	Vect3f l_PosI, l_PosF;
	float sizeI;
	float sizeF;
	m_LinesCount = m_NumPuntos + 2;
	float l_Distance = l_Dir.Length() / m_LinesCount;
	l_Dir.Normalize();

	l_PosF = m_PosInicial;
	l_PosF.y = UABEngine.GetRandomValue(m_PosInicial.y + m_OffsetY, m_PosInicial.y - m_OffsetY);
	sizeF = UABEngine.GetRandomValue(m_Size + m_SizeOffset, m_Size - m_SizeOffset);
	for (int i = 0; i < m_LinesCount-1; ++i)
	{
		l_PosI = l_PosF;
		l_PosF = m_PosInicial + l_Dir*l_Distance*(i+1);
		l_PosF.y = UABEngine.GetRandomValue(l_PosF.y + m_OffsetY, l_PosF.y - m_OffsetY);
		sizeI = sizeF;
		sizeF = UABEngine.GetRandomValue(m_Size + m_SizeOffset, m_Size - m_SizeOffset);
		m_LineRenderableData[i].Position = Vect4f(l_PosI, l_PosF.x);
		m_LineRenderableData[i].UV = Vect2f(l_PosF.y, l_PosF.z);
		m_LineRenderableData[i].UV2 = Vect2f(sizeI, sizeF);
		m_LineRenderableData[i].Color = m_Color;
	}	
	m_LineRenderableData[m_LinesCount-1].Position = Vect4f(l_PosF, m_PosFinal.x);
	m_LineRenderableData[m_LinesCount-1].UV = Vect2f(UABEngine.GetRandomValue(m_PosFinal.y + m_OffsetY, m_PosFinal.y - m_OffsetY), m_PosFinal.z);
	m_LineRenderableData[m_LinesCount-1].UV2 = Vect2f(sizeF, UABEngine.GetRandomValue(m_Size + m_SizeOffset, m_Size - m_SizeOffset));
	m_LineRenderableData[m_LinesCount-1].Color = m_Color;
}

void CLineRenderer::Render(CRenderManager *RM)
{
	RM->GetContextManager()->SetWorldMatrix(GetTransform());
	CEffectTechnique * l_ET = m_Material->GetRenderableObjectTechnique()->GetEffectTechnique();
	m_Material->Apply();
	CEffectManager::SetSceneConstants(l_ET);
	m_RenderableVertex->UpdateVertexs(m_LineRenderableData, MAX_PARTICLE_PER_INSTANCE);
	m_RenderableVertex->Render(RM, l_ET, CEffectManager::GetRawData(), m_LinesCount); 
}