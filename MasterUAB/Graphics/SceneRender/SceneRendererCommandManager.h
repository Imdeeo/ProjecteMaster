#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"

class CRenderManager;
class CLevel;

class CSceneRendererCommandManager : public	CTemplatedVectorMapManager<CSceneRendererCommand>
{
private:
	std::string m_LevelId;
	std::string m_Filename;
public:
	CSceneRendererCommandManager();
	virtual ~CSceneRendererCommandManager();
	std::string GetNextName();
	bool Load(const std::string &Filename, CLevel* _Level);
	bool Reload();
	void Execute(CRenderManager *RenderManager);
};

#endif //SCENE_RENDERER_COMMAND_MANAGER_H