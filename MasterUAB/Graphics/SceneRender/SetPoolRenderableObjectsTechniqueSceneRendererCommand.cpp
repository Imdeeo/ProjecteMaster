#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\PoolRenderableObjectTechnique.h"

#include "XML\XMLTreeNode.h"

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_Pool = UABEngine.GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(TreeNode.GetPszProperty("pool"));
}

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::~CSetPoolRenderableObjectsTechniqueSceneRendererCommand(){}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Pool->Apply();
}