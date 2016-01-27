#include "SceneRendererCommandManager.h"
#include "RenderManager\RenderManager.h"
#include "XML\XMLTreeNode.h"
#include "SetDepthStencilStateSceneRendererCommand.h"
#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "SetMatricesSceneRendererCommand.h"
#include "ClearSceneRendererCommand.h"
#include "LightsSceneRendererCommand.h"
#include "RenderLayerSceneRendererCommand.h"
#include "PresentSceneRendererCommand.h"
#include "RenderDebugLightsSceneRendererCommand.h"

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
				else if (l_Element.GetName() == std::string("set_matrices"))
				{
					AddResource(l_Element.GetName(), new CSetMatricesSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("clear"))
				{
					AddResource(l_Element.GetName(), new CClearSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_light_constants"))
				{
					AddResource(l_Element.GetName(), new CLightsSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_layer"))
				{
					AddResource(l_Element.GetName(), new CRenderLayerSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("present"))
				{
					AddResource(l_Element.GetName(), new CPresentSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_debug_grid"))
				{
					//AddResource(l_Element.GetName(), new CRenderDebubGridSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_debug_lights"))
				{
					AddResource(l_Element.GetName(), new CRenderDebugLightsSceneRendererCommand(l_Element));
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
