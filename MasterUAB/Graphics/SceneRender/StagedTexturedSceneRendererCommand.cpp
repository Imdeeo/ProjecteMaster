#include "StagedTexturedSceneRendererCommand.h"

#include "Engine\UABEngine.h"
#include "Materials\MaterialManager.h"
#include "Texture\TextureManager.h"

#include "Texture\DynamicTextureManager.h"
#include "Texture\DynamicTexture.h"
#include "Texture\CapturedFrameBufferTexture.h"
#include "Texture\Texture.h"

#include <d3d11.h>

CStagedTexturedSceneRendererCommand::CStagedTexturedSceneRendererCommand(tinyxml2::XMLElement* TreeNode) :CSceneRendererCommand(TreeNode)
{
	m_CapturedFrameBufferTexture = nullptr;
	tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement();
	while (l_Element != NULL)
	{
		if (l_Element->Name() == std::string("texture"))
		{
			unsigned int l_StagedId = l_Element->GetIntProperty("stage_id");
			std::string l_TextureFile = l_Element->GetPszProperty("file");
			//todo if load_file

			AddStageTexture(l_StagedId,UABEngine.GetTextureManager()->GetTexture(l_TextureFile));
		}
		else if(l_Element->Name() == std::string("dynamic_texture"))
		{
			AddDynamicTexture(l_Element);
		}
		else if(l_Element->Name() == std::string("capture_texture"))
		{
			AddCaptureFrameBufferTexture(l_Element);
		}
		l_Element = l_Element->NextSiblingElement();
	}
	CreateRenderTargetViewVector();
}

CStagedTexturedSceneRendererCommand::~CStagedTexturedSceneRendererCommand(void)
{	
	m_StagedTextures.clear();	

	for(size_t i=0; i<m_DynamicTextures.size(); ++i)
	{
		UABEngine.GetTextureManager()->RemoveResource(m_DynamicTextures[i]->GetTexture()->GetName());
		CHECKED_DELETE(m_DynamicTextures[i]);
	}

	m_DynamicTextures.clear();

	if (m_CapturedFrameBufferTexture != nullptr)
	{
		UABEngine.GetTextureManager()->RemoveResource(m_CapturedFrameBufferTexture->GetName());
	}
}

void CStagedTexturedSceneRendererCommand::ActivateTextures()
{
	for(size_t i = 0; i<m_StagedTextures.size();i++)
	{
		m_StagedTextures[i].Activate();
	}
}

void CStagedTexturedSceneRendererCommand::AddStageTexture(unsigned int _StagedId, CTexture* _Texture)
{
	m_StagedTextures.push_back(CStagedTexture(_StagedId, _Texture));
}

void CStagedTexturedSceneRendererCommand::AddDynamicTexture(tinyxml2::XMLElement* TreeNode)
{
	std::string l_Name = TreeNode->GetPszProperty("name");
	if (TreeNode->GetPszProperty("material") != NULL)
		m_Material = UABEngine.GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
	CDynamicTexture* l_DynamicTexture = new CDynamicTexture(TreeNode);
	m_DynamicTextures.push_back(new CDynamicTextureManager(l_DynamicTexture,m_Material));
	UABEngine.GetTextureManager()->AddResource(l_Name, l_DynamicTexture);
}

void CStagedTexturedSceneRendererCommand::AddCaptureFrameBufferTexture(tinyxml2::XMLElement* TreeNode)
{
	m_CapturedFrameBufferTexture = new CCapturedFrameBufferTexture(TreeNode);
	UABEngine.GetTextureManager()->AddUpdateResource(m_CapturedFrameBufferTexture->GetName(),m_CapturedFrameBufferTexture);
}

void CStagedTexturedSceneRendererCommand::CreateRenderTargetViewVector()
{
	for(size_t i = 0; i<m_DynamicTextures.size(); ++i)
	{
		m_RenderTargetViews.push_back(m_DynamicTextures[i]->GetTexture()->GetRenderTargetView());
	}
}

//StagedTexture
CStagedTexturedSceneRendererCommand::CStagedTexture::CStagedTexture(unsigned int _StageId, CTexture *_Texture)
{
	m_StageId = _StageId;
	m_Texture = _Texture;
}
void CStagedTexturedSceneRendererCommand::CStagedTexture::Activate()
{
	m_Texture->Activate(m_StageId);
}