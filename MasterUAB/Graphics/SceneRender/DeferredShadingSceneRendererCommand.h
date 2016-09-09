#ifndef DEFFERRED_SHADING_SCENE_RENDER_H
#define DEFFERRED_SHADING_SCENE_RENDER_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderableObjectTechnique;
class ID3D11BlendState;

class CDeferredShadingSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	ID3D11BlendState *m_EnabledAlphaBlendState;
public:
	CDeferredShadingSceneRendererCommand(tinyxml2::XMLElement* TreeNode);
	virtual ~CDeferredShadingSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //DEFFERRED_SHADING_SCENE_RENDER_H