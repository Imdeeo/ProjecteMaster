#ifndef APPLY_FILTERS_SCENE_RENDERER_COMMAND_H
#define APPLY_FILTERS_SCENE_RENDERER_COMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderManager;

class CApplyFiltersSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
public:
	CApplyFiltersSceneRendererCommand(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	virtual ~CApplyFiltersSceneRendererCommand(){}
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //APPLY_FILTERS_SCENE_RENDERER_COMMAND_H