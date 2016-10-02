#include "SpotLight.h"

#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"
#include "RenderManager\RenderManager.h"
#include "DebugRender.h"
#include "Camera\Frustum.h"
#include "Texture\DynamicTexture.h"
#include "RenderableObjects\RenderableObjectsManager.h"

CSpotLight::CSpotLight(const std::string &_LevelId) :CDirectionalLight(_LevelId), m_Angle(0.0f), m_FallOff(0.0f){}

CSpotLight::CSpotLight(std::string _name, const std::string &_LevelId) : CDirectionalLight(_name,_LevelId), m_Angle(0.0f), m_FallOff(0.0f)
{
	m_Type = GetLightTypeByName("spot");
	m_ShadowMap = new CDynamicTexture("shadowmap", 512, 512, true, "r32");
	m_ShadowMaskTexture = nullptr;
}

CSpotLight::CSpotLight(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CDirectionalLight(TreeNode,_LevelId)
{
	m_Angle = TreeNode->GetFloatProperty("angle",1.f);
	m_FallOff = TreeNode->GetFloatProperty("fall_off", 1.2f);
	m_Rotation.SetFromScaledAxis(m_Direction);
}

#ifdef _DEBUG
void CSpotLight::Render(CRenderManager *_RenderManager)
{
	if (GetEnabled())
	{
		CDirectionalLight::Render(_RenderManager);
	}
}
#endif


void getYawPitch(Vect3f v1, double &_Yaw, double &_Pitch, double &_Roll)
{
	_Pitch = acos((v1.GetNormalized()*Vect3f(0, -1, 0)));
	//v1.y = 0;
	_Yaw = asin(v1.GetNormalized()*Vect3f(1.f, 0.f, 0.f));

	_Roll = asin(v1.GetNormalized()*Vect3f(0, 0, 1));
	if (_Roll < 0)
	{
		_Pitch = -_Pitch;
		//_Yaw = -_Yaw;
		_Roll = -_Roll;
	}
}

const Mat44f & CSpotLight::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	float l_Intensity = GetIntensity();
	float l_factor = tanf(m_Angle*0.5f);
	if (l_factor > 1.f)
	{
		m_ScaleMatrix.Scale(l_Intensity, l_Intensity / l_factor, l_Intensity);
	}
	else
	{
		m_ScaleMatrix.Scale(l_factor*l_Intensity, l_Intensity, l_factor*l_Intensity);
	}

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix = m_Rotation.rotationMatrix();

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

	return m_TransformMatrix;
}

void CSpotLight::SetShadowMap(CRenderManager &RenderManager)
{
	if (m_ShadowMap == NULL)
		return;
	m_ViewShadowMap.SetIdentity();
	Vect3f up = Vect3f(m_Direction.z, m_Direction.y, m_Direction.x);
	up = ((up) ^ (m_Direction));
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_Direction, up.y < 0 ? (up * -1):up);
	unsigned int l_ShadowMapWidth = m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight = m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetIdentity();
	m_ProjectionShadowMap.SetFromPerspective(m_FallOff, l_ShadowMapWidth / (float)l_ShadowMapHeight, 0.1f, m_EndRangeAttenuation);
	CEffectManager::m_SceneParameters.m_View = m_ViewShadowMap;
	CEffectManager::m_SceneParameters.m_Projection = m_ProjectionShadowMap;
	ID3D11RenderTargetView *l_RenderTargetViews[1];
	l_RenderTargetViews[0] = m_ShadowMap->GetRenderTargetView();
	D3D11_VIEWPORT m_viewport;
	m_viewport.Width = (float)l_ShadowMapWidth;
	m_viewport.Height = (float)l_ShadowMapHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	RenderManager.GetDeviceContext()->RSSetViewports(1, &m_viewport);
	RenderManager.SetRenderTargets(1, l_RenderTargetViews, m_ShadowMap->GetDepthStencilView());
}

#ifdef _DEBUG
CRenderableVertexs* CSpotLight::GetShape(CRenderManager *_RenderManager)
{
	return _RenderManager->GetDebugRender()->GetCone();
}
#endif

bool const CSpotLight::GetInsideFrustum()
{
	return m_Frustum->SphereVisible(m_Position + (m_Direction.GetNormalized() * m_EndRangeAttenuation) / 2, m_EndRangeAttenuation);
}

void CSpotLight::Save(FILE* _File)
{
	if (m_GenerateShadowMap)
	{ 
		fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"spot\" pos=\"%f %f %f\" dir=\"%f %f %f\" color=\"%f %f %f %f\" angle=\"%f\" fall_off=\"%f\" att_start_range=\"%f\" "
			"att_end_range=\"%f\" intensity=\"%f\" generate_shadow_map=\"true\" shadow_map_width=\"%i\" shadow_map_height=\"%i\" shadow_map_format=\"%s\" shadow_texture_mask=\"%s\">\n",
			m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Direction.x, m_Direction.y, m_Direction.z, m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(),
			m_Angle, m_FallOff, m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity, m_ShadowMap->GetWidth(), m_ShadowMap->GetHeight(), m_ShadowMap->GetFormat().c_str(), 
			m_ShadowMaskTexture != NULL ? m_ShadowMaskTexture->GetName().c_str() : "");
		for (size_t i = 0; i < m_Layers.size(); ++i)
			fprintf_s(_File, "\t\t<layer layer=\"%s\"/>\n", m_Layers[i]->GetName().c_str());
		fprintf_s(_File, "\t</light>\n");
	}
	else
	{
		fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"spot\" pos=\"%f %f %f\" dir=\"%f %f %f\" color=\"%f %f %f %f\" angle=\"%f\" fall_off=\"%f\" "
			"att_start_range=\"%f\" att_end_range=\"%f\" intensity=\"%f\" generate_shadow_map=\"false\"/>\n",
			m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Direction.x, m_Direction.y, m_Direction.z, m_Color.GetRed(), 
			m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(), m_Angle, m_FallOff, m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity);
	}
}
