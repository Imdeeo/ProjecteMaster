#include "DrawQuadRendererCommand.h"

#include "RenderManager/RenderManager.h"
#include "RenderableObjects/RenderableObjectTechnique.h"
#include "Effects/EffectTechnique.h"
#include "Materials\Material.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Materials\MaterialManager.h"
#include "Utils.h"

#include "XML\XMLTreeNode.h"

#include "Math\Color.h"


CDrawQuadRendererCommand::CDrawQuadRendererCommand(tinyxml2::XMLElement* TreeNode) :CStagedTexturedSceneRendererCommand(TreeNode)
{
	const char* c = TreeNode->GetPszProperty("material", "");
	if (c != "")
	{
		m_Material = UABEngine.GetMaterialManager()->GetResource(std::string(c));
		m_RenderableObjectTechnique = m_Material->GetRenderableObjectTechnique();
	}
	else
	{
		m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
		m_Material = nullptr;
	}
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand(void)
{
}

void CDrawQuadRendererCommand::Execute(CRenderManager &_RenderManager)
{
	if (GetActive())
	{
		for (size_t i = 0; i < m_StagedTextures.size(); i++)
			m_StagedTextures[i].Activate();

		if (m_Material != nullptr)
			m_Material->Apply(m_RenderableObjectTechnique);

		_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
	}
}
