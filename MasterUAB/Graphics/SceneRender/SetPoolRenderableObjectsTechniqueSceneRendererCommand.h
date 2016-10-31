#ifndef SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H
#define SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CPoolRenderableObjectTechnique;

class CLevel;

class CSetPoolRenderableObjectsTechniqueSceneRendererCommand : public CSceneRendererCommand
{
private:
	CPoolRenderableObjectTechnique *m_Pool;
public:
	CSetPoolRenderableObjectsTechniqueSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CSetPoolRenderableObjectsTechniqueSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H