#include "SceneRendererCommandManager.h"
#include "RenderManager.h"

CSceneRendererCommandManager::CSceneRendererCommandManager()
{
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{

}

std::string CSceneRendererCommandManager::GetNextName()
{
	return NULL;
}

bool CSceneRendererCommandManager::Load(const std::string &Filename)
{
	return false;
}

bool CSceneRendererCommandManager::Reload()
{
	return false;
}

void CSceneRendererCommandManager::Execute(CRenderManager &RenderManager)
{

}