#ifndef STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H
#define STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H

#include "SceneRender\SceneRendererCommand.h"
#include "Texture\Texture.h"
#include <vector>

class CDynamicTexture;

class CStagedTexturedSceneRendererCommand :
	public CSceneRendererCommand
{
protected:
	class CStagedTexture
	{
	public:
		unsigned int m_StageId;
		CTexture * m_Texture;

		CStagedTexture(unsigned int _StageId, CTexture *_Texture)
		{
			m_StageId = _StageId;
			m_Texture = _Texture;
		}
		void Activate()
		{
			m_Texture->Activate(m_StageId);
		}
	};

	std::vector<CStagedTexture>				m_StagedTextures;
	std::vector<CDynamicTexture*>			m_DynamicTextures;
	std::vector<ID3D11RenderTargetView*>	m_RenderTargetViews;

public:
	CStagedTexturedSceneRendererCommand(CXMLTreeNode & TreeNode);
	virtual ~CStagedTexturedSceneRendererCommand(void);

	void CreateRenderTargetViewVector();
	void ActivateTextures();
	void AddStageTexture(unsigned int _StageId, CTexture* _Texture);
	virtual void Execute(CRenderManager &_RenderManager) = 0;


};

#endif //STAGED_TEXTURED_SCENE_RENDERER_COMMAND_H