#include "SceneRendererCommandManager.h"
#include "RenderManager\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "SetDepthStencilStateSceneRendererCommand.h"
#include "SetPoolRenderableObjectsTechniqueSceneRend.h"

CSceneRendererCommandManager::CSceneRendererCommandManager(){}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
	Destroy();
}

std::string CSceneRendererCommandManager::GetNextName()
{
	return NULL;
}

bool CSceneRendererCommandManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["scene_renderer_commands"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				
				if (l_Element.GetName() == std::string("set_depth_stencil_state"))
				{
					AddResource(l_Element.GetName(), new CSetDepthStencilStateSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_pool_renderable_objects_technique"))
				{
					AddResource(l_Element.GetName(), new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("effect_technique"))
				{
					/*l_EffectName = l_Element.GetPszProperty("name");
					CEffectTechnique *l_EffectTechnique = new CEffectTechnique(l_Element);
					AddResource(l_EffectName, l_EffectTechnique);*/
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool CSceneRendererCommandManager::Reload()
{
	return Load(m_Filename);
}

void CSceneRendererCommandManager::Execute(CRenderManager &RenderManager)
{
	// TODO
}
