#ifndef	DRAW_QUAD_RENDERER_COMMAND_H
#define DRAW_QUAD_RENDERER_COMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderableObjectTechnique;
class CMaterial;
class CLevel;

class CDrawQuadRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique* m_RenderableObjectTechnique;
public:
	CDrawQuadRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	virtual ~CDrawQuadRendererCommand(void);

	void Execute(CRenderManager &RenderManager);
};

#endif //DRAW_QUAD_RENDERER_COMMAND_H