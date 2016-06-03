#include "DirectionalLight.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ContextManager\ContextManager.h"
#include "Effects\EffectManager.h"
#include "DebugRender.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "StaticMesh\InstanceMesh.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "Camera\CameraControllerManager.h"
#include "Texture\DynamicTexture.h"
#include "Math\Vector3.h"

CDirectionalLight::CDirectionalLight() : CLight(), m_Direction(Vect3f(0.0f, 0.0f, 0.0f))
{
}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	m_Direction = TreeNode.GetVect3fProperty("dir",Vect3f(0.0,0.0,0.0));
	m_Rotation.SetFromScaledAxis(m_Direction);
	if (m_GenerateShadowMap)
	{
		m_OrthoShadowMapSize = TreeNode.GetVect2fProperty("ortho_size", Vect2f(50, 50), true);
	}
}

#ifdef _DEBUG
void CDirectionalLight::Render(CRenderManager *RenderManager)
{
	if (m_Enabled)
	{
		CLight::Render(RenderManager);
		CRenderableVertexs* l_Line = RenderManager->GetDebugRender()->GetLine(m_Position, m_Position + (GetDirection() * GetEndRangeAttenuation()));
		RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		CEffectTechnique* l_EffectTechnique = UABEngine.GetRenderableObjectTechniqueManager()->GetResource("debug_lights")->GetEffectTechnique();
		CEffectManager::SetSceneConstants(l_EffectTechnique);
		l_Line->Render(RenderManager, l_EffectTechnique, CEffectManager::GetRawData());
		delete l_Line;
	}
}
#endif

const Mat44f & CDirectionalLight::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(GetIntensity(), GetIntensity(), GetIntensity());

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix = m_Rotation.rotationMatrix();

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

	return m_TransformMatrix;
}

void CDirectionalLight::SetShadowMap(CRenderManager &RenderManager)
{
	m_ViewShadowMap.SetIdentity();
	CCameraController* l_auxCameraController = UABEngine.GetCameraControllerManager()->GetMainCamera();
	float l_Half_Range = m_EndRangeAttenuation*0.5;
	m_Position = l_auxCameraController->GetPosition() + l_auxCameraController->GetForward().GetNormalized() * l_Half_Range*0.5f - GetDirection().GetNormalized() * l_Half_Range*0.5f;
	Vect3f up = Vect3f(m_Direction.z, m_Direction.y, m_Direction.x);
	up = ((up) ^ (m_Direction));
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_Direction, up.y < 0 ? (up * -1) : up);
	unsigned int l_ShadowMapWidth = m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight = m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetFromOrtho(m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, -l_Half_Range, l_Half_Range);
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

void CDirectionalLight::Save(FILE* _File)
{
	if (m_GenerateShadowMap)
	{
		fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"directional\" pos=\"%f %f %f\" dir=\"%f %f %f\" color=\"%f %f %f %f\" att_start_range=\"%f\" att_end_range=\"%f\" intensity=\"%f\" "
			"ortho_size=\"%f %f\" generate_shadow_map=\"true\" shadow_map_width=\"%i\" shadow_map_height=\"%i\" shadow_map_format=\"%s\" shadow_texture_mask=\"%s\">\n",
			m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Direction.x, m_Direction.y, m_Direction.z, m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(),
			m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, m_ShadowMap->GetWidth(), m_ShadowMap->GetHeight(), m_ShadowMap->GetFormat().c_str(), 
			m_ShadowMaskTexture != NULL ? m_ShadowMaskTexture->GetName().c_str() : "");
	}
	else
	{
		fprintf_s(_File, "\t<light name=\"%s\" enabled=\"%s\" type=\"directional\" pos=\"%f %f %f\" dir=\"%f %f %f\" color=\"%f %f %f %f\" att_start_range=\"%f\" "
			"att_end_range=\"%f\" intensity=\"%f\" ortho_size=\"%f %f\" generate_shadow_map=\"false\">\n",
			m_Name.c_str(), m_Enabled ? "true" : "false", m_Position.x, m_Position.y, m_Position.z, m_Direction.x, m_Direction.y, m_Direction.z, 
			m_Color.GetRed(), m_Color.GetGreen(), m_Color.GetBlue(), m_Color.GetAlpha(), m_StartRangeAttenuation, m_EndRangeAttenuation, m_Intensity, 
			m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y);		
	}

	for (size_t i = 0; i < m_Layers.size(); ++i)
		fprintf_s(_File, "\t\t<layer layer=\"%s\"/>\n", m_Layers[i]->GetName().c_str());
	fprintf_s(_File, "\t</light>\n");
}

