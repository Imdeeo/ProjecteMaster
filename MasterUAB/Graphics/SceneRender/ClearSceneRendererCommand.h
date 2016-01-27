#ifndef CLEAR_SCENE_RENDERER_COMMAND_H
#define CLEAR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CRenderManager;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
protected:
	bool m_RenderTarget;
	bool m_DepthStencil;
public:
	CClearSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //CLEAR_SCENE_RENDERER_COMMAND_H