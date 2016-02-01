#ifndef SCENE_RENDERER_COMMAND_MANAGER_H
#define SCENE_RENDERER_COMMAND_MANAGER_H

#include <string>
#include "Utils\TemplatedVectorMapManager.h"

class CSceneRendererCommand;
class CRenderManager;

class CSceneRendererCommandManager : public	CTemplatedVectorMapManager<CSceneRendererCommand>
{
private:
	std::string m_Filename;
public:
	CSceneRendererCommandManager();
	virtual ~CSceneRendererCommandManager();
	std::string GetNextName();
	bool Load(const std::string &Filename);
	bool Reload();
	void Execute(CRenderManager *RenderManager);
};

#endif //SCENE_RENDERER_COMMAND_MANAGER_H