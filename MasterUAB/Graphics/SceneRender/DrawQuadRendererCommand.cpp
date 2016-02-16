#include "DrawQuadRendererCommand.h"

#include "RenderManager/RenderManager.h"
#include "RenderableObjects/RenderableObjectTechnique.h"
#include "Effects/EffectTechnique.h"
#include "Materials\Material.h"
#include "Engine\UABEngine.h"


CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	const char* c = l_Element.GetPszProperty("material");
	if (c != NULL)
	{
		m_Material = UABEngine.GetMaterialManager()->GetResource(std::string(c));
		m_RenderableObjectTechnique = m_Material->GetRenderableObjectTechnique();
	}
	else
	{
		m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
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
			m_StagedTextures[i].Activate();
		m_Material->Apply(m_RenderableObjectTechnique);
		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
}
