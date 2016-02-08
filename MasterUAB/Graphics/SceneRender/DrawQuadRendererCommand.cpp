#include "DrawQuadRendererCommand.h"

#include "RenderManager/RenderManager.h"
#include "RenderableObjects/RenderableObjectTechnique.h"
#include "Effects/EffectTechnique.h"
#include "Engine\UABEngine.h"


CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	const char* c = l_Element.GetPszProperty("material");
	if (c != NULL)
	{
		m_RenderableObjectTechnique = UABEngine.GetMaterialManager()->GetResource(std::string(c))->GetRenderableObjectTechnique();
	}
	else
	{
		m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("2d");
	}
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand(void)
{
}

void CDrawQuadRendererCommand::Execute(CRenderManager &_RenderManager)
{
	if (Getactive())
	{
		for (int i = 0; i < m_StagedTextures.size(); i++)
		{
			_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), m_StagedTextures[i].m_Texture, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}
