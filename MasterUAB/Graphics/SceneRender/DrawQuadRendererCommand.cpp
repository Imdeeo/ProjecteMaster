#include "DrawQuadRendererCommand.h"

#include "RenderManager/RenderManager.h"
#include "RenderableObjects/RenderableObjectTechnique.h"
#include "Effects/EffectTechnique.h"
#include "Engine\UABEngine.h"


CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	const char* c = l_Element.GetPszProperty("renderable_object_technique");
	if (c != NULL)
	{
		std::string l_TechniqueName = std::string(c);
		m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_TechniqueName);
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
