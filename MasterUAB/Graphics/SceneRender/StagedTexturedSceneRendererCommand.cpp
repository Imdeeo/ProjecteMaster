#include "StagedTexturedSceneRendererCommand.h"
#include "Engine\UABEngine.h"

#include "Texture\DynamicTexture.h"
#include "Texture\CapturedFrameBufferTexture.h"

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode & TreeNode):CSceneRendererCommand(TreeNode)
{
	CXMLTreeNode l_Input = TreeNode;
	for (int i = 0; i < l_Input.GetNumChildren(); i++)
	{
		CXMLTreeNode l_Element = l_Input(i);
		if (l_Element.GetName() == std::string("texture"))
		{
			unsigned int l_StagedId = l_Element.GetIntProperty("stage_id");
			std::string l_TextureFile = l_Element.GetPszProperty("file");
			//todo if load_file
			AddStageTexture(l_StagedId,UABEngine.GetTextureManager()->GetTexture(l_TextureFile));
		}
		else if(l_Element.GetName() == std::string("dynamic_texture"))
		{
			AddDynamicTexture(l_Element);
		}
		else if(l_Element.GetName() == std::string("capture_texture"))
		{
			AddCaptureFrameBufferTexture(l_Element);
		}
	}
	CreateRenderTargetViewVector();
}

CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand(void)
{
}

void CStagedTexturedSceneRendererCommand::ActivateTextures()
{
	for(int i = 0; i<m_StagedTextures.size();i++)
	{
		m_StagedTextures[i].Activate();
	}
}

void CStagedTexturedSceneRendererCommand::AddStageTexture(unsigned int _StagedId, CTexture* _Texture)
{
	m_StagedTextures.push_back(CStagedTexture(_StagedId, _Texture));
}

void CStagedTexturedSceneRendererCommand::AddDynamicTexture(CXMLTreeNode & TreeNode)
{
	m_DynamicTextures.push_back(new CDynamicTexture(TreeNode));
}

void CStagedTexturedSceneRendererCommand::AddCaptureFrameBufferTexture(CXMLTreeNode & TreeNode)
{
}

void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	for(int i = 0; m_DynamicTextures.size(); ++i)
	{
		m_RenderTargetViews.push_back(m_DynamicTextures[i]->GetRenderTargetView());
	}
}