#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "RenderManager\RenderManager.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\PoolRenderableObjectTechnique.h"


CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel *_Level) : CSceneRendererCommand(TreeNode,_Level)
{
	m_Pool = UABEngine.GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(TreeNode->GetPszProperty("pool"));
}

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::~CSetPoolRenderableObjectsTechniqueSceneRendererCommand(){}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Pool->Apply();
}