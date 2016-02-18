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
#include "RenderAntTweakBarSceneRendereCommand.h"
#include "RenderGridSceneRendererCommand.h"
#include "DrawQuadRendererCommand.h"
#include "SetRenderTargetSceneRendererCommand.h"
#include "UnsetRenderTargetSceneRendererCommand.h"
#include "DeferredShadingSceneRendererCommand.h"
#include "ApplyFiltersSceneRendererCommand.h"
#include "CaptureFrameBufferSceneRendererCommand.h"
#include "GenerateShadowMapsSceneRendererCommand.h"
#include "EnableAlphaBlendSceneRendererCommand.h"
#include "DisableAlphaBlendSceneRendererCommand.h"

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
	Destroy();
	m_Filename = Filename;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["scene_renderer_commands"];
		if (l_Input.Exists())
		{
			
			std::string l_Name;
			for (unsigned int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				const char* c = l_Element.GetPszProperty("name");
				if(c==(const char*)0)
				{
#ifdef _VS13
					l_Name = std::string(l_Element.GetName())+std::to_string(i);
#else
					char buffer[30];
					itoa(i,buffer,10);
					l_Name = std::string(l_Element.GetName()+std::string(buffer));
#endif
				}
				else
				{
					l_Name = std::string(c);
				}

				if (l_Element.GetName() == std::string("set_depth_stencil_state"))
				{
					AddResource(l_Name, new CSetDepthStencilStateSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_pool_renderable_objects_technique"))
				{
					AddResource(l_Name, new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_matrices"))
				{
					AddResource(l_Name, new CSetMatricesSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("clear"))
				{
					AddResource(l_Name, new CClearSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_light_constants"))
				{
					AddResource(l_Name, new CLightsSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_layer"))
				{
					AddResource(l_Name, new CRenderLayerSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("present"))
				{
					AddResource(l_Name, new CPresentSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_debug_grid"))
				{
					AddResource(l_Name, new CRenderGridSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_debug_lights"))
				{
					AddResource(l_Name, new CRenderDebugLightsSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_ant_tweak_bar"))
				{
					AddResource(l_Name, new CRenderAntTweakBarSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_draw_quad"))
				{
					AddResource(l_Name, new CDrawQuadRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("set_render_target"))
				{
					AddResource(l_Name, new CSetRenderTargetSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("unset_render_target"))
				{
					AddResource(l_Name, new CUnsetRenderTargetSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("render_deferred_shading"))
				{
					AddResource(l_Name, new CDeferredShadingSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("apply_filters"))
				{
					AddResource(l_Name, new CApplyFiltersSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("capture_frame_buffer"))
				{
					AddResource(l_Name, new CCaptureFrameBufferSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("generate_shadow_maps"))
				{
					AddResource(l_Name, new CGenerateShadowMapsSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("enable_alpha_blend"))
				{
					AddResource(l_Name, new CEnableAlphaBlendSceneRendererCommand(l_Element));
				}
				else if (l_Element.GetName() == std::string("disable_alpha_blend"))
				{
					AddResource(l_Name, new CDisableAlphaBlendSceneRendererCommand(l_Element));
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

void CSceneRendererCommandManager::Execute(CRenderManager *RenderManager)
{
	for(int i=0;i<m_ResourcesVector.size();++i)
	{
		m_ResourcesVector[i]->Execute(*RenderManager);
	}
}