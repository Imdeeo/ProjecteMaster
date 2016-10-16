#include "RenderDebugLayerSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Layers\LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "LevelManager\LevelManager.h"


CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) :CSceneRendererCommand(TreeNode,_LevelId)
{
	m_Layer = UABEngine.GetLevelManager()->GetResource(_LevelId)->GetLayerManager()->GetLayer(TreeNode);
	SetActive(TreeNode->GetBoolProperty("active", false));
}

CRenderDebugLayerSceneRendererCommand::~CRenderDebugLayerSceneRendererCommand()
{
	//CHECKED_RELEASE(m_Layer);
}

void CRenderDebugLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
#ifdef _DEBUG
	m_Layer->RenderDebug(&RenderManager);
#endif
}