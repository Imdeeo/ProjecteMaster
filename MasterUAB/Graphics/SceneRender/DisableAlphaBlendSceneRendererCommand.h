#pragma once
#include "SceneRendererCommand.h"
class CDisableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{

public:
	CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CDisableAlphaBlendSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager);
};

