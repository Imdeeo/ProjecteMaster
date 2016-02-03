#ifndef SCENE_RENDERER_COMMAND_H
#define SCENE_RENDERER_COMMAND_H

#include "Utils\Named.h"
#include "XML\XMLTreeNode.h"
#include "Materials\Material.h"
#include "Utils\Active.h"

class CRenderManager;

class CSceneRendererCommand :public CActive, public CNamed
{
protected:
	CMaterial *m_Material;
public:
	CSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CSceneRendererCommand();
	virtual void Execute(CRenderManager &RenderManager) = 0;
};

#endif //SCENE_RENDERER_COMMAND_H