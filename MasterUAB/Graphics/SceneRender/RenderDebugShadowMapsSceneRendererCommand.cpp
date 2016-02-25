#include "RenderDebugShadowMapsSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"

CRenderDebugShadowMapsSceneRendererCommand::CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_Offset = TreeNode.GetBoolProperty("offset");
	m_Width = TreeNode.GetBoolProperty("width");
	
	if (TreeNode.GetFloatProperty("light") != NULL)
		m_Light = TreeNode.GetFloatProperty("light");

	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("2d_technique");
}

CRenderDebugShadowMapsSceneRendererCommand::~CRenderDebugShadowMapsSceneRendererCommand()
{
}

void CRenderDebugShadowMapsSceneRendererCommand::Execute(CRenderManager & _RenderManager){
	if (m_Light != NULL)
	{
		UABEngine.GetLightManager()->GetResourcesVector()[m_Light]->GetShadowMap()->Activate(0);
	}
	else
	{
		for (size_t i = 0; i < UABEngine.GetLightManager()->GetResourcesVector().size(); ++i)
		{
			UABEngine.GetLightManager()->GetResourcesVector()[i]->GetShadowMap()->Activate(i);
		}
	}	

	_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
}