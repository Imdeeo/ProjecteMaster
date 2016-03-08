#ifndef STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H
#define STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H

#include "SceneRender\SceneRendererCommand.h"

#include <vector>

class CDynamicTextureManager;
class CCapturedFrameBufferTexture;
class CTexture;

class ID3D11RenderTargetView;

class CStagedTexturedSceneRendererCommand : public CSceneRendererCommand
{
protected:
	class CStagedTexture
	{
	public:
		unsigned int m_StageId;
		CTexture * m_Texture;

		CStagedTexture(unsigned int _StageId, CTexture *_Texture);
		void Activate();
	};

	std::vector<CStagedTexture>				m_StagedTextures;
	std::vector<CDynamicTextureManager*>	m_DynamicTextures;
	CCapturedFrameBufferTexture*			m_CapturedFrameBufferTexture;
	std::vector<ID3D11RenderTargetView*>	m_RenderTargetViews;

public:
	CStagedTexturedSceneRendererCommand(CXMLTreeNode & TreeNode);
	virtual ~CStagedTexturedSceneRendererCommand(void);

	void CreateRenderTargetViewVector();
	void ActivateTextures();
	void AddStageTexture(unsigned int _StageId, CTexture* _Texture);
	void AddDynamicTexture(CXMLTreeNode & TreeNode);
	void AddCaptureFrameBufferTexture(CXMLTreeNode & TreeNode);
	virtual void Execute(CRenderManager &_RenderManager) = 0;
};

#endif //STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H