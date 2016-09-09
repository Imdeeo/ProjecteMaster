#ifndef	H_DRAW_VIDEO_QUAD_RENDERER_COMMAND_H
#define H_DRAW_VIDEO_QUAD_RENDERER_COMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderableObjectTechnique;
class CMaterial;
class CDynamicTexture;

class CDrawVideoQuadRendererCommand : public CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique* m_RenderableObjectTechnique;
	CDynamicTexture *m_Texture;
public:
	CDrawVideoQuadRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CDrawVideoQuadRendererCommand(void);

	void Execute(CRenderManager &RenderManager);
};

#endif //DRAW_QUAD_RENDERER_COMMAND_H