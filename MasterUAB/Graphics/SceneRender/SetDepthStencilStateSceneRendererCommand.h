#ifndef SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H
#define SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"


class ID3D11DepthStencilState;
class CRenderManager;
class CXMLTreeNode;

class CSetDepthStencilStateSceneRendererCommand : public CSceneRendererCommand
{
private:
	ID3D11DepthStencilState *m_DepthStencilState;
public:
	CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSetDepthStencilStateSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H