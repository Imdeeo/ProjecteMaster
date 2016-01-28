#include "StagedTexturedSceneRendererCommand.h"
#include "Engine\UABEngine.h"

#include "Texture\DynamicTexture.h"

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode & TreeNode):CSceneRendererCommand(TreeNode)
{
	CXMLTreeNode l_Element = TreeNode;
	for (int i = 0; i < l_Element.GetNumChildren(); i++)
	{
		CXMLTreeNode l_Child = l_Element(i);
		if (l_Child.GetName() == "texture")
		{
			unsigned int l_StagedId = l_Child.GetIntProperty("stage_id");
			std::string l_TextureFile = l_Child.GetPszProperty("file");
			AddStageTexture(l_StagedId,UABEngine.GetTextureManager()->GetTexture(l_TextureFile));
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


void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	//TODO
}