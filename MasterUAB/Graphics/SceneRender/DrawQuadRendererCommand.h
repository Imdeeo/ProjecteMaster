#ifndef	DRAW_QUAD_RENDERER_COMMAND_H
#define DRAW_QUAD_RENDERER_COMMAND_H

#include "scenerenderercommand.h"

class CDrawQuadRendererCommand :
	public CSceneRendererCommand
{
public:
	CDrawQuadRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CDrawQuadRendererCommand(void);

	void Execute(CRenderManager &RenderManager);
};

#endif //DRAW_QUAD_RENDERER_COMMAND_H