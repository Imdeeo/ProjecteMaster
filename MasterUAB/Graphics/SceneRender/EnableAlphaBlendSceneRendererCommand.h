#ifndef ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H
#define ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H

#include "SceneRendererCommand.h"
class CEnableAlphaBlendSceneRendererCommand :
	public CSceneRendererCommand
{
private:
	std::vector<D3D11_BLEND> m_Blend;

	srcBlend;
	destBlend;
	opBlend;

	AlphasrcBlend;
	AlphadestBlend;
	AlphaopBlend;

	D3D11_BLEND CheckBlendType(std::string _type);
public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual ~CEnableAlphaBlendSceneRendererCommand();

	virtual void Execute(CRenderManager &RenderManager);
};

#endif //ENABLE_ALPHA_BLEND_SCENE_RENDERER_COMMAND_H