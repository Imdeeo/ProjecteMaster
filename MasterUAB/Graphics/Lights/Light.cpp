#include "Light.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "DebugRender.h"

#include "Layers\LayerManager.h"
#include "Effects\EffectManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableVertexs.h"

#include "Texture\DynamicTexture.h"
#include "Camera\Frustum.h"
#include "LevelManager\LevelManager.h"

//#include "RenderManager\RenderManager.h"

CLight::CLight(CLevel* _Level) : CNamed(""), CLevelInfo(_Level), C3DElement(Vect3f(0, 0, 0)), m_Type(LIGHT_TYPE_OMNI), m_Color(Vect4f(1.0f, 1.0f, 1.0f, 1.0f)), m_StartRangeAttenuation(0.0f), m_EndRangeAttenuation(0.0f), m_Intensity(0.0f), m_Enabled(false), m_GenerateShadowMap(false), m_ShadowMap(nullptr)
{
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
}

CLight::CLight(std::string _name, CLevel* _Level) : CNamed(_name), CLevelInfo(_Level), C3DElement(Vect3f(0, 0, 0)), m_Type(LIGHT_TYPE_OMNI), m_Color(Vect4f(1.0f, 1.0f, 1.0f, 1.0f)), m_StartRangeAttenuation(0.0f), m_EndRangeAttenuation(0.0f), m_Intensity(0.0f), m_Enabled(false), m_GenerateShadowMap(false), m_ShadowMap(nullptr)
{	
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
}

CLight::CLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level) : CNamed(TreeNode), C3DElement(TreeNode), CLevelInfo(_Level)
{
	m_Frustum = UABEngine.GetRenderManager()->GetFrustum();
	m_Type = GetLightTypeByName(TreeNode->GetPszProperty("type"));
	//m_Position = TreeNode.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f), true);
	m_Color = CColor(TreeNode->GetVect4fProperty("color", Vect4f(255.0f, 255.0f, 255.0f, 1.0f)));
	m_StartRangeAttenuation = TreeNode->GetFloatProperty("att_start_range");
	m_EndRangeAttenuation = TreeNode->GetFloatProperty("att_end_range");
	m_Intensity = TreeNode->GetFloatProperty("intensity");
	m_Enabled = TreeNode->GetBoolProperty("enabled");
	m_GenerateShadowMap = TreeNode->GetBoolProperty("generate_shadow_map");
	
	if (m_GenerateShadowMap){
		tinyxml2::XMLElement* l_Element = TreeNode->FirstChildElement("layer");
		if (l_Element!=NULL)
		{
			if (l_Element->Name() == std::string("layer"))
			{
				m_Layers.push_back(_Level->GetLayerManager()->GetResource(l_Element->GetPszProperty("layer")));
			}
		}
		m_ShadowMap = new CDynamicTexture("shadowmap", (int)TreeNode->GetFloatProperty("shadow_map_width"), (int)TreeNode->GetFloatProperty("shadow_map_height"), true, TreeNode->GetPszProperty("shadow_map_format"));
		//m_ShadowMaskTexture = new CTexture();
		m_ShadowMaskTexture = nullptr;
	}
	else
	{
		m_ShadowMap = nullptr;
		m_ShadowMaskTexture = nullptr;
	}
}

CLight::~CLight(){
	Destroy();
}

void CLight::Destroy() {
	CHECKED_DELETE(m_ShadowMap);
}

#ifdef _DEBUG
void CLight::Render(CRenderManager *_RenderManager)
{
	if (m_Enabled)
	{
		CEffectManager::m_SceneParameters.m_BaseColor = m_Color*m_Intensity;
		CEffectManager::m_SceneParameters.m_BaseColor.SetAlpha(1.f);
		_RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
		CEffectManager::SetSceneConstants(l_EffectTechnique);
		GetShape(_RenderManager)->RenderIndexed(_RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
	}
}
#endif

CRenderableVertexs* CLight::GetShape(CRenderManager *_RenderManager)
{
	#ifdef _DEBUG
		return _RenderManager->GetDebugRender()->GetSPhere10();
	#else
		return nullptr;
	#endif
}


CLight::TLightType CLight::GetLightTypeByName(const std::string &StrLightType)
{
	if(StrLightType=="omni")
		return LIGHT_TYPE_OMNI;
	else if (StrLightType=="spot")
		return LIGHT_TYPE_SPOT;
	else if (StrLightType=="directional")
		return LIGHT_TYPE_DIRECTIONAL;
	return LIGHT_TYPE_NULL;
}

const Mat44f & CLight::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(m_Intensity*0.25f, m_Intensity*0.25f, m_Intensity*0.25f);

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix = m_Rotation.rotationMatrix();

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

	return m_TransformMatrix;
}

const bool CLight::GetInsideFrustum()
{
	return m_Frustum->SphereVisible(m_Position, m_EndRangeAttenuation);
}

#ifdef _DEBUG

#endif

CDynamicTexture* CLight::GetShadowMap()
{
	return m_ShadowMap;
}
CTexture* CLight::GetShadowMaskTexture()
{
	return m_ShadowMaskTexture;
}