#ifndef SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H
#define SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CPoolRenderableObjectTechnique;

class CSetPoolRenderableObjectsTechniqueSceneRendererCommand : public CSceneRendererCommand
{
private:
	CPoolRenderableObjectTechnique *m_Pool;
public:
	CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_POOL_RENDERABLE_OBJECTS_TECHNIQUE_SCENE_REND_H