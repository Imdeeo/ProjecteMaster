#include "RenderDebugShadowMapsSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Texture\DynamicTexture.h"

#include "RenderableObjects\RenderableObjectTechnique.h"

#include "XML\XMLTreeNode.h"

CRenderDebugShadowMapsSceneRendererCommand::CRenderDebugShadowMapsSceneRendererCommand(tinyxml2::XMLElement* TreeNode) : CSceneRendererCommand(TreeNode)
{
	//m_Offset = TreeNode.GetBoolProperty("offset");
	//m_Width = TreeNode.GetBoolProperty("width");
	
	m_Light = TreeNode->GetIntProperty("light");

	m_RenderableObjectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_NORMAL_TEXTURE_VERTEX");
}

CRenderDebugShadowMapsSceneRendererCommand::~CRenderDebugShadowMapsSceneRendererCommand()
{
}

void CRenderDebugShadowMapsSceneRendererCommand::Execute(CRenderManager & _RenderManager){
	UABEngine.GetLightManager()->GetResourcesVector()[m_Light]->GetShadowMap()->Activate(0);
	_RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1, 1, CColor(1.f, 1.f, 1.f, 1.f));
}