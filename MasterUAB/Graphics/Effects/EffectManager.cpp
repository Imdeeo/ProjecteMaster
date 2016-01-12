#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "SceneEffectParameters.h";
#include "AnimatedModelEffectParameters.h";
#include "LightEffectParameters.h";
#include "Lights\Light.h";
#include "Lights\DirectionalLight.h";
#include "Lights\SpotLight.h";
#include "Engine\UABEngine.h"

CEffectManager::CEffectManager(void)
{
}

CEffectManager::~CEffectManager(void)
{
}

void CEffectManager::Reload()
{
	m_VertexShaders.Destroy();
	m_PixelShaders.Destroy();
	CTemplatedMapManager::Destroy();
	Load(m_Filename);
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
		CXMLTreeNode l_Input = l_XML["effects"];
		if (l_Input.Exists())
		{
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
CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return m_VertexShaders[VertexShader];
}
CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return m_PixelShaders[PixelShader];
}

void CEffectManager::SetSceneConstants()
{
	//m_SceneParameters.m_World = UABEngine.GetRenderManager()->GetContextManager()->;
}

void CEffectManager::SetLightConstants(unsigned int IdLight, CLight *Light)
{	
	m_LightParameters.m_LightAmbient[IdLight] = (0.1f, 0.1f, 0.1f, 1.0f);
	m_LightParameters.m_LightEnabled[IdLight] = Light->GetEnabled()?1:0;
	m_LightParameters.m_LightType[IdLight] = Light->GetType();
	m_LightParameters.m_LightPosition[IdLight] = Light->GetPosition();;	
	m_LightParameters.m_LightAttenuationStartRange[IdLight] = Light->GetStartRangeAttenuation();
	m_LightParameters.m_LightAttenuationEndRange[IdLight] = Light->GetEndRangeAttenuation();
	m_LightParameters.m_LightIntensity[IdLight] = Light->GetIntensity();
	m_LightParameters.m_LightColor[IdLight] = Light->GetColor();

	switch (Light->GetType())
	{	
	case CLight::LIGHT_TYPE_DIRECTIONAL:
		m_LightParameters.m_LightDirection[IdLight] = dynamic_cast<CDirectionalLight*>(Light)->GetDirection();
		break;
	case CLight::LIGHT_TYPE_SPOT:
		m_LightParameters.m_LightAngle[IdLight] = dynamic_cast<CSpotLight*>(Light)->GetAngle();
		m_LightParameters.m_LightFallOffAngle[IdLight] = dynamic_cast<CSpotLight*>(Light)->GetFallOff();
		break;
	case CLight::LIGHT_TYPE_OMNI:
	default:
		break;
	}	
}

void CEffectManager::SetLightsConstants(unsigned int MaxLights)
{
	int n_lights = UABEngine.GetLightManager()->GetResourcesVector().size();
	
	for (size_t i = 0; i < MaxLights; i++)
	{
		if(n_lights<=i)
		{
			CLight* dummy = new CDirectionalLight();
			dummy->SetEnabled(false);
			SetLightConstants(i,dummy);
			delete dummy;
		}
		else
		{
			SetLightConstants(i, UABEngine.GetLightManager()->GetResourceById(i));
		}
	}
}