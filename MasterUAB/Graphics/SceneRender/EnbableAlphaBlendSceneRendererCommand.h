#pragma once
#include "SceneRendererCommand.h"
class CEnbableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CEnbableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CEnbableAlphaBlendSceneRendererCommand();

	virtual void Execute(CRenderManager &RenderManager);
};

