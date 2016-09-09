#ifndef	DRAW_QUAD_RENDERER_COMMAND_H
#define DRAW_QUAD_RENDERER_COMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderableObjectTechnique;
class CMaterial;

class CDrawQuadRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique* m_RenderableObjectTechnique;
public:
	CDrawQuadRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CDrawQuadRendererCommand(void);

	void Execute(CRenderManager &RenderManager);
};

#endif //DRAW_QUAD_RENDERER_COMMAND_H