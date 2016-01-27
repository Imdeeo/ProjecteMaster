#pragma once
#include "SceneRendererCommand.h"
class CRenderGridSceneRendererCommand :
	public CSceneRendererCommand
{
public:
	CRenderGridSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CRenderGridSceneRendererCommand();

	void Execute(CRenderManager &RenderManager);
};

