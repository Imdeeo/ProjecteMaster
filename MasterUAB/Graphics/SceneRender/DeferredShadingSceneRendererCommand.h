#ifndef DEFFERRED_SHADING_SCENE_RENDER_H
#define DEFFERRED_SHADING_SCENE_RENDER_H

#include "StagedTexturedSceneRendererCommand.h"

class CRenderableObjectTechnique;
class ID3D11BlendState;
class CLevel;

class CDeferredShadingSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	ID3D11BlendState *m_EnabledAlphaBlendState;
public:
	CDeferredShadingSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CDeferredShadingSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //DEFFERRED_SHADING_SCENE_RENDER_H