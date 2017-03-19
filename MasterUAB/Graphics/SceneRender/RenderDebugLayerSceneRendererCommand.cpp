#include "RenderDebugLayerSceneRendererCommand.h"
#include "Engine\UABEngine.h"
#include "Layers\LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "LevelManager\LevelManager.h"


CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level) :CSceneRendererCommand(TreeNode,_Level)
{
	m_Layer = _Level->GetLayerManager()->GetLayer(TreeNode);
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