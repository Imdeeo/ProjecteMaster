#include "RenderDebugLayerSceneRendererCommand.h"

#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "RenderableObjects\RenderableObjectsManager.h"


CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode):CSceneRendererCommand(TreeNode)
{
}

CRenderDebugLayerSceneRendererCommand::~CRenderDebugLayerSceneRendererCommand()
{
	//CHECKED_RELEASE(m_Layer);
}

void Execute(CRenderManager &RenderManager)
{
}