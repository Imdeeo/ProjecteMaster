#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Lights\Light.h";
#include "Lights\DirectionalLight.h";
#include "Lights\SpotLight.h";
#include "Engine\UABEngine.h";
#include "EffectShader.h"
#include "DebugHelper\DebugHelper.h"

static void __stdcall ReloadEffect(void* _app)
{
	CEffectTechnique* l_technique = (CEffectTechnique*)_app;
	l_technique->GetVertexShader()->Reload();
	l_technique->GetPixelShader()->Reload();
}

CEffectManager::CEffectManager(void)
{
}

CEffectManager::~CEffectManager(void)
{
}

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

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CDebugHelper::GetDebugHelper()->RemoveBar("Effects:");
		CDebugHelper::SDebugBar l_effects_bar;
		CXMLTreeNode l_Input = l_XML["effects"];
		if (l_Input.Exists())
		{
			l_effects_bar.name = "Effects:";

			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("vertex_shader"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectVertexShader *l_EffectVertexShader = new CEffectVertexShader(l_Element);
					l_EffectVertexShader->Load();
					m_VertexShaders.AddResource(l_EffectName, l_EffectVertexShader);
				} else if (l_Element.GetName() == std::string("pixel_shader"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectPixelShader *l_EffectPixelShader = new CEffectPixelShader(l_Element);
					l_EffectPixelShader->Load();
					m_PixelShaders.AddResource(l_EffectName, l_EffectPixelShader);
				} else if (l_Element.GetName() == std::string("effect_technique"))
				{
					l_EffectName = l_Element.GetPszProperty("name");
					CEffectTechnique *l_EffectTechnique = new CEffectTechnique(l_Element);
					AddResource(l_EffectName, l_EffectTechnique);

					{
						CDebugHelper::SDebugVariable var = {};
						var.name = l_EffectName;
						var.type = CDebugHelper::BUTTON;
						var.callback = ReloadEffect;
						var.data = l_EffectTechnique;

						l_effects_bar.variables.push_back(var);
					}
				}
			}
		}
		CDebugHelper::GetDebugHelper()->RegisterBar(l_effects_bar);
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

void CEffectManager::SetSceneConstants(CEffectTechnique* _EffectTechnique)
{
	ID3D11DeviceContext* l_DeviceContext = UABEngine.GetRenderManager()->GetDeviceContext();
	ID3D11Buffer *l_SceneConstantBufferVS = _EffectTechnique->GetVertexShader()->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
	l_DeviceContext->UpdateSubresource(l_SceneConstantBufferVS, 0, NULL, &(CEffectManager::m_SceneParameters), 0, 0);
	ID3D11Buffer *l_SceneConstantBufferPS = _EffectTechnique->GetPixelShader()->GetConstantBuffer(SCENE_CONSTANT_BUFFER_ID);
	l_DeviceContext->UpdateSubresource(l_SceneConstantBufferPS, 0, NULL, &(CEffectManager::m_SceneParameters), 0, 0);
}

void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{	
	m_LightParameters.m_LightEnabled[IdLight] = Light->GetEnabled()?1:0;
	m_LightParameters.m_LightType[IdLight] = Light->GetType();
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
		l_ShadowMap->Activate(INDEX_SHADOWMAP_TEXTURE);
		if (l_ShadowMask != NULL)
			l_ShadowMask->Activate(INDEX_SHADOWMAP_TEXTURE+1);
	}
	else
	{
		CEffectManager::m_LightParameters.m_UseShadowMap[IdLight] = 0.0f;
	}
}

void CEffectManager::SetLightsConstants(unsigned int MaxLights)
{
	int n_lights = UABEngine.GetLightManager()->GetResourcesVector().size();
	m_LightParameters.m_LightAmbient = UABEngine.GetLightManager()->GetAmbientLight();
	for (size_t i = 0; i < MaxLights; i++)
	{
		if(n_lights<=i)
		{
			CLight* dummy = new CDirectionalLight();
			dummy->SetEnabled(false);
			dummy->SetGenerateShadowMap(false);
			SetLightConstants(i,dummy);
			delete dummy;
		}
		else
		{
			SetLightConstants(i, UABEngine.GetLightManager()->GetResourceById(i));
		}
	}
	CEffectManager::m_LightParameters.m_FogColor = Vect4f(UABEngine.GetLightManager()->GetFogColor(),1.0f);
	float maxAttenuation =
		UABEngine.GetLightManager()->GetFogEnabled() ? UABEngine.GetLightManager()->GetFogMaxAttenuation() : 0.f;
	CEffectManager::m_LightParameters.m_MaxAttenuation_StartLinearFog_EndLinearFog = 
		Vect4f(maxAttenuation, UABEngine.GetLightManager()->GetFogStart(),
		UABEngine.GetLightManager()->GetFogEnd(),1.0f);

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
	return l_Adress;
}
