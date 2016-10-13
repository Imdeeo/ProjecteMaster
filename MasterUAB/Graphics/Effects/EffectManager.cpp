#include "Effects\EffectManager.h"
#include "XML\tinyxml2.h"

#include "Lights\Light.h"
#include "Lights\DirectionalLight.h"
#include "Lights\SpotLight.h"

#include "Texture\DynamicTexture.h"

#include "LevelManager\LevelManager.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "Lights\LightManager.h"

#include "DebugHelper\DebugHelper.h"

CEffectManager::CEffectManager(void){}

CEffectManager::~CEffectManager(void){}

void CEffectManager::ReloadShader()
{
	typedef  std::map<std::string, CEffectVertexShader*>::iterator it_type;
	for (it_type iterator = m_VertexShaders.GetResourcesMap().begin(); iterator != m_VertexShaders.GetResourcesMap().end(); iterator++)
	{
		iterator->second->Reload();
	}
	
	typedef  std::map<std::string, CEffectPixelShader*>::iterator it_type2;
	for (it_type2 iterator2 = m_PixelShaders.GetResourcesMap().begin(); iterator2 != m_PixelShaders.GetResourcesMap().end(); iterator2++)
	{
		iterator2->second->Reload();
	}
}

void CEffectManager::Reload()
{
	ReloadShader();
	typedef  std::map<std::string, CEffectTechnique*>::iterator it_type;
	for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
	{
		iterator->second->Refresh();
	}
}

CSceneEffectParameters CEffectManager::m_SceneParameters;
CAnimatedModelEffectParameters CEffectManager::m_AnimatedModelEffectParameters;
CLightEffectParameters CEffectManager::m_LightParameters;

bool CEffectManager::Load(const std::string &Filename)
{
	m_Filename = Filename;
	std::string l_EffectName;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError l_Error = doc.LoadFile(Filename.c_str());

	tinyxml2::XMLElement* l_Element;
	tinyxml2::XMLElement* l_ElementAux;


	if (l_Error == tinyxml2::XML_SUCCESS)
	{
		l_Element = doc.FirstChildElement("effects");
		if (l_Element != NULL)
		{
			l_ElementAux = l_Element->FirstChildElement();
			while (l_ElementAux!= NULL)
			{
				if (l_ElementAux->Name() == std::string("vertex_shader"))
				{
					l_EffectName = l_ElementAux->GetPszProperty("name");
					CEffectVertexShader *l_EffectVertexShader = new CEffectVertexShader(l_ElementAux);
					l_EffectVertexShader->Load();
					m_VertexShaders.AddResource(l_EffectName, l_EffectVertexShader);
				} else if (l_ElementAux->Name() == std::string("pixel_shader"))
				{
					l_EffectName = l_ElementAux->GetPszProperty("name");
					CEffectPixelShader *l_EffectPixelShader = new CEffectPixelShader(l_ElementAux);
					l_EffectPixelShader->Load();
					m_PixelShaders.AddResource(l_EffectName, l_EffectPixelShader);
				}
				else if (l_ElementAux->Name() == std::string("geometry_shader"))
				{
					l_EffectName = l_ElementAux->GetPszProperty("name");
					CEffectGeometryShader *l_EffectGeometryShader = new CEffectGeometryShader(l_ElementAux);
					l_EffectGeometryShader->Load();
					m_GeometryShaders.AddResource(l_EffectName, l_EffectGeometryShader);
				}
				else if (l_ElementAux->Name() == std::string("effect_technique"))
				{
					l_EffectName = l_ElementAux->GetPszProperty("name");
					CEffectTechnique *l_EffectTechnique = new CEffectTechnique(l_ElementAux);
					AddResource(l_EffectName, l_EffectTechnique);
				}
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
CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders[VertexShader];
}
CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders[PixelShader];
}

CEffectGeometryShader * CEffectManager::GetGeometryShader(const std::string &GeometryShader)
{
	return m_GeometryShaders[GeometryShader];
}

void CEffectManager::SetSceneConstants(CEffectTechnique* _EffectTechnique)
{
	if (_EffectTechnique->GetVertexShader() == NULL || _EffectTechnique->GetPixelShader() == NULL)
		return;
	ID3D11DeviceContext* l_DeviceContext = UABEngine.GetRenderManager()->GetDeviceContext();
	ID3D11Buffer *l_SceneConstantBufferVS = _EffectTechnique->GetVertexShader()->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
	if (l_SceneConstantBufferVS == NULL)
		return;
	l_DeviceContext->UpdateSubresource(l_SceneConstantBufferVS, 0, NULL, &(CEffectManager::m_SceneParameters), 0, 0);
	ID3D11Buffer *l_SceneConstantBufferPS = _EffectTechnique->GetPixelShader()->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
	if (l_SceneConstantBufferPS == NULL)
		return;
	l_DeviceContext->UpdateSubresource(l_SceneConstantBufferPS, 0, NULL, &(CEffectManager::m_SceneParameters), 0, 0);
	if (_EffectTechnique->GetGeometryShader() != nullptr)
	{
		ID3D11Buffer *l_SceneConstantBufferGS = _EffectTechnique->GetGeometryShader()->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
		l_DeviceContext->UpdateSubresource(l_SceneConstantBufferGS, 0, NULL, &(CEffectManager::m_SceneParameters), 0, 0);
	}
}

void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{	
	m_LightParameters.m_LightEnabled[IdLight] = Light->GetEnabled()?1.0f:0.0f;
	m_LightParameters.m_LightType[IdLight] = (float)Light->GetType();
	m_LightParameters.m_LightPosition[IdLight] = Light->GetPosition();
	m_LightParameters.m_LightAttenuationStartRange[IdLight] = Light->GetStartRangeAttenuation();
	m_LightParameters.m_LightAttenuationEndRange[IdLight] = Light->GetEndRangeAttenuation();
	m_LightParameters.m_LightIntensity[IdLight] = Light->GetIntensity();
	m_LightParameters.m_LightColor[IdLight] = Light->GetColor();

	
	switch (Light->GetType())
	{	
		case CLight::LIGHT_TYPE_SPOT:
			m_LightParameters.m_LightAngle[IdLight] = dynamic_cast<CSpotLight*>(Light)->GetAngle();
			m_LightParameters.m_LightFallOffAngle[IdLight] = dynamic_cast<CSpotLight*>(Light)->GetFallOff();
		case CLight::LIGHT_TYPE_DIRECTIONAL:
			m_LightParameters.m_LightDirection[IdLight] = dynamic_cast<CDirectionalLight*>(Light)->GetDirection();
			break;
		case CLight::LIGHT_TYPE_OMNI:
		default:
			break;
	}

	if (Light->GetGenerateShadowMap())
	{
		CDynamicTexture *l_ShadowMap = Light->GetShadowMap();
		CTexture *l_ShadowMask = Light->GetShadowMaskTexture();
		CEffectManager::m_LightParameters.m_UseShadowMap[IdLight] = 1.0f;
		CEffectManager::m_LightParameters.m_UseShadowMask[IdLight] = l_ShadowMask != NULL ? 1.0f : 0.0f;
		CEffectManager::m_LightParameters.m_LightView[IdLight] = Light->GetViewShadowMap();
		CEffectManager::m_LightParameters.m_LightProjection[IdLight] = Light->GetProjectionShadowMap();
		if (l_ShadowMap != NULL)
		{
			l_ShadowMap->Activate(INDEX_SHADOWMAP_TEXTURE);
		}
		else
		{
			Light->CreateShadowMap( new CDynamicTexture("shadowmap", 1024, 1024, true, "r32"));
			l_ShadowMap = Light->GetShadowMap();
			l_ShadowMap->Activate(INDEX_SHADOWMAP_TEXTURE);
		}
		if (l_ShadowMask != NULL)
			l_ShadowMask->Activate(INDEX_SHADOWMAP_TEXTURE+1);
	}
	else
	{
		CEffectManager::m_LightParameters.m_UseShadowMap[IdLight] = 0.0f;
	}
}

void CEffectManager::SetLightsConstants(unsigned int MaxLights,const std::string &_LevelId)
{
	CLightManager *l_LightManager = UABEngine.GetLevelManager()->GetResource(_LevelId)->GetLightManager();
	int n_lights = l_LightManager->GetResourcesVector().size();
	m_LightParameters.m_LightAmbient = l_LightManager->GetAmbientLight();
	for (size_t i = 0; i < MaxLights; i++)
	{
		if((size_t)n_lights<=i)
		{
			CLight* dummy = new CDirectionalLight("Dummy");
			dummy->SetEnabled(false);
			dummy->SetGenerateShadowMap(false);
			SetLightConstants(i,dummy);
			delete dummy;
		}
		else
		{
			SetLightConstants(i, l_LightManager->GetResourceById(i));
		}
	}
	
	std::map<std::string, CEffectTechnique*>::iterator it;
	std::map<std::string, CEffectTechnique*>::iterator end =GetResourcesMap().end();
	for (it = GetResourcesMap().begin(); it != end; it++)
	{
		ID3D11Buffer * l_LightConstantBufferVS = it->second->GetVertexShader()->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		ID3D11Buffer * l_LightConstantBufferPS = it->second->GetPixelShader()->GetConstantBuffer(LIGHT_CONSTANT_BUFFER_ID);
		if (l_LightConstantBufferVS != NULL)
		{
			UABEngine.GetRenderManager()->GetDeviceContext()->UpdateSubresource(l_LightConstantBufferVS, 0, NULL, &(CEffectManager::m_LightParameters), 0, 0);
		}
		if (l_LightConstantBufferPS != NULL)
		{
			UABEngine.GetRenderManager()->GetDeviceContext()->UpdateSubresource(l_LightConstantBufferPS, 0, NULL, &(CEffectManager::m_LightParameters), 0, 0);
		}
	}
}

int CEffectManager::m_RawDataCount = 0;
float CEffectManager::m_RawData[MAX_RAW_DATA_ELEMENTS];
void* CEffectManager::AddMaterialParameter(CMaterialParameter::TMaterialType _MaterialType)
{
	if (m_RawDataCount == MAX_RAW_DATA_ELEMENTS)
	{
		return nullptr;
	}
	void* l_Adress = &(m_RawData[m_RawDataCount]);
	if(_MaterialType == CMaterialParameter::FLOAT)
		m_RawDataCount = m_RawDataCount + 1;
	if(_MaterialType == CMaterialParameter::VECT2F)
		m_RawDataCount = m_RawDataCount + 2;
	if(_MaterialType == CMaterialParameter::VECT3F)
		m_RawDataCount = m_RawDataCount + 3;
	if(_MaterialType == CMaterialParameter::VECT4F)
		m_RawDataCount = m_RawDataCount + 4;
	if (_MaterialType == CMaterialParameter::COLOR)
		m_RawDataCount == m_RawDataCount + 4;
	return l_Adress;
}

luabind::object CEffectManager::GetEffectsNames(lua_State *L)
{
	luabind::object l_ElementsVector = luabind::newtable(L);
	typedef TMapResource::iterator it_type;
	size_t i = 0;
	for (it_type iterator = m_Resources.begin(); iterator != m_Resources.end(); iterator++)
	{
		l_ElementsVector[i++] = iterator->second->GetName().c_str();
	}
	return l_ElementsVector;
}