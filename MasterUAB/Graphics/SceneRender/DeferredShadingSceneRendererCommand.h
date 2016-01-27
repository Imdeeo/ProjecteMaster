#ifndef DEFFERRED_SHADING_SCENE_RENDER_H
#define DEFFERRED_SHADING_SCENE_RENDER_H

#include "SceneRendererCommand.h"
#include "SceneRender\CStagedTexturedSceneRendererCommand"
class CDeferredShadingSceneRendererCommand : public
	CStagedTexturedSceneRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	ID3D11BlendState *m_EnabledAlphaBlendState;
public:
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CDeferredShadingSceneRendererCommand();
	void Execute(CRenderManager &RenderManager);
};

#endif //DEFFERRED_SHADING_SCENE_RENDER_H