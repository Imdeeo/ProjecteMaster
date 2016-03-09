#ifndef CLEAR_SCENE_RENDERER_COMMAND_H
#define CLEAR_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"

class CRenderManager;
class CXMLTreeNode;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
private:
	bool m_RenderTarget;
	bool m_DepthStencil;
public:
	CClearSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CClearSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

#endif //CLEAR_SCENE_RENDERER_COMMAND_H