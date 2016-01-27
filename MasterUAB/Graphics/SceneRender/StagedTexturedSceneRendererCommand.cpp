#include "StagedTexturedSceneRendererCommand.h"


CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(CXMLTreeNode & TreeNode):CSceneRendererCommand(TreeNode)
{
	//TODO
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