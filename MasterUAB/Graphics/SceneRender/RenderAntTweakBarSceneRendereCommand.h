#ifndef RENDER_ANTTWEAKBAR_SCENE_RENDERER_COMMAND_H
#define RENDER_ANTTWEAKBAR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CLevel;

class CRenderAntTweakBarSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderAntTweakBarSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CRenderAntTweakBarSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //RENDER_ANTTWEAKBAR_SCENE_RENDERER_COMMAND_H