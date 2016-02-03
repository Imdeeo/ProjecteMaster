#ifndef SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H
#define SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
#include "D3D11.h"

class CRenderManager;

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