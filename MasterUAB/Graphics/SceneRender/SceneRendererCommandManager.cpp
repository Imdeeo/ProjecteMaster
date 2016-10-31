#include "SceneRendererCommandManager.h"
#include "RenderManager\RenderManager.h"
#include "SetDepthStencilStateSceneRendererCommand.h"
#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "SetMatricesSceneRendererCommand.h"
#include "ClearSceneRendererCommand.h"
#include "LightsSceneRendererCommand.h"
#include "RenderLayerSceneRendererCommand.h"
#include "PresentSceneRendererCommand.h"
#include "RenderDebugLightsSceneRendererCommand.h"
#include "RenderDebugFrustumSceneRendererCommand.h"
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
#include "RenderAxisSceneRenderCommand.h"
#include "SetLightPositionSceneRendererCommand.h"
#include "RenderDebugLayerSceneRendererCommand.h"
#include "RenderDebugShadowMapsSceneRendererCommand.h"
#include "RenderGUISceneRendererCommand.h"
#include "DrawVideoQuadRenderCommand.h"
#include "RenderPathFindingSceneRendererCommand.h"

#include "LevelManager\Level.h"
#include "Engine\UABEngine.h"

CSceneRendererCommandManager::CSceneRendererCommandManager(){}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
	Destroy();
}

std::string CSceneRendererCommandManager::GetNextName()
{
	return NULL;
}

bool CSceneRendererCommandManager::Load(const std::string &Filename,CLevel* _Level)
{
	m_Filename = Filename;
	m_LevelId = _Level->GetName();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;

	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("scene_renderer_commands");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			std::string l_Name;
			int i = 0;
			while (l_ElementAux != NULL)
			{
				const char* c = l_ElementAux->GetPszProperty("name");
				if(c==(const char*)0)
				{
#ifdef _VS13
					l_Name = std::string(l_ElementAux->Name()) + std::to_string(i);
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

				if (l_ElementAux->Name() == std::string("set_depth_stencil_state"))
				{
					AddResource(l_Name, new CSetDepthStencilStateSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_pool_renderable_objects_technique"))
				{
					AddResource(l_Name, new CSetPoolRenderableObjectsTechniqueSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_matrices"))
				{
					AddResource(l_Name, new CSetMatricesSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("clear"))
				{
					AddResource(l_Name, new CClearSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_light_constants"))
				{
					AddResource(l_Name, new CLightsSceneRendererCommand(l_ElementAux, _Level));
				}
				else if(l_ElementAux->Name() == std::string("render_layer"))
				{
					AddResource(l_Name, new CRenderLayerSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("present"))
				{
					AddResource(l_Name, new CPresentSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_grid"))
				{
					AddResource(l_Name, new CRenderGridSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_axis"))
				{
					AddResource(l_Name, new CRenderAxisSceneRenderCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_lights"))
				{
					AddResource(l_Name, new CRenderDebugLightsSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_frustum"))
				{
					AddResource(l_Name, new CRenderDebugFrustumSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_layer"))
				{
					AddResource(l_Name, new CRenderDebugLayerSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_ant_tweak_bar"))
				{
					AddResource(l_Name, new CRenderAntTweakBarSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_draw_quad"))
				{
					AddResource(l_Name, new CDrawQuadRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_render_target"))
				{
					AddResource(l_Name, new CSetRenderTargetSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("unset_render_target"))
				{
					AddResource(l_Name, new CUnsetRenderTargetSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_deferred_shading"))
				{
					AddResource(l_Name, new CDeferredShadingSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("apply_filters"))
				{
					AddResource(l_Name, new CApplyFiltersSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("capture_frame_buffer"))
				{
					AddResource(l_Name, new CCaptureFrameBufferSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("generate_shadow_maps"))
				{
					AddResource(l_Name, new CGenerateShadowMapsSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("enable_alpha_blend"))
				{
					AddResource(l_Name, new CEnableAlphaBlendSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("disable_alpha_blend"))
				{
					AddResource(l_Name, new CDisableAlphaBlendSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("set_light_position"))
				{
					AddResource(l_Name, new CSetLightPositionSceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_shadowmap"))
				{
					AddResource(l_Name, new CRenderDebugShadowMapsSceneRendererCommand(l_ElementAux, _Level));
				}			
				else if (l_ElementAux->Name() == std::string("render_gui"))
				{
					AddResource(l_Name, new CRenderGUISceneRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_video"))
				{
					AddResource(l_Name, new CDrawVideoQuadRendererCommand(l_ElementAux, _Level));
				}
				else if (l_ElementAux->Name() == std::string("render_debug_pathfinding"))
				{
					AddResource(l_Name, new CRenderPathFindingSceneRendererCommand(l_ElementAux, _Level));
				}
				++i;
				l_ElementAux = l_ElementAux->NextSiblingElement();
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
	Destroy();
	return Load(m_Filename,UABEngine.GetLevelManager()->GetResource(m_LevelId));
}

void CSceneRendererCommandManager::Execute(CRenderManager *RenderManager)
{
	for(size_t i=0;i<m_ResourcesVector.size();++i)
	{
		if (m_ResourcesVector[i]->GetActive())
		{
			m_ResourcesVector[i]->Execute(*RenderManager);
		}
	}
}