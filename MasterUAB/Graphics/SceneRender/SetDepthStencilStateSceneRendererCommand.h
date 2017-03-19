#ifndef SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H
#define SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"


class ID3D11DepthStencilState;
class CRenderManager;
class CXMLTreeNode;
class CLevel;

class CSetDepthStencilStateSceneRendererCommand : public CSceneRendererCommand
{
private:
	ID3D11DepthStencilState *m_DepthStencilState;
public:
	CSetDepthStencilStateSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CSetDepthStencilStateSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //SET_DEPTH_STENCIL_SCENE_RENDERER_COMMAND_H