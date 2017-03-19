#ifndef STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H
#define STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H

#include "SceneRender\SceneRendererCommand.h"

#include <vector>

class CDynamicTextureManager;
class CCapturedFrameBufferTexture;
class CTexture;

class ID3D11RenderTargetView;
class CLevel;

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
	CStagedTexturedSceneRendererCommand(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	virtual ~CStagedTexturedSceneRendererCommand(void);

	void CreateRenderTargetViewVector();
	void ActivateTextures();
	void AddStageTexture(unsigned int _StageId, CTexture* _Texture);
	void AddDynamicTexture(tinyxml2::XMLElement* TreeNode,CLevel*_Level);
	void AddCaptureFrameBufferTexture(tinyxml2::XMLElement* TreeNode);
	virtual void Execute(CRenderManager &_RenderManager) = 0;
};

#endif //STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H