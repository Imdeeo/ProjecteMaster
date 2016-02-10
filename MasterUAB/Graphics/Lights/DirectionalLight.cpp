#include "DirectionalLight.h"

#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"

#include "XML\XMLTreeNode.h"

CDirectionalLight::CDirectionalLight() : CLight(),m_Direction(Vect3f(0.0f,0.0f,0.0f)) {}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	m_Direction = TreeNode.GetVect3fProperty("dir",Vect3f(0.0,0.0,0.0));
}

void CDirectionalLight::Render(CRenderManager *RenderManager)
{
	CLight::Render(RenderManager);
}
const Mat44f & CDirectionalLight::GetTransform()
{
	m_ScaleMatrix.SetIdentity();
	m_ScaleMatrix.Scale(GetIntensity(), GetIntensity(), GetIntensity());

	m_RotationMatrix.SetIdentity();
	m_RotationMatrix.SetFromLookAt(m_Position,m_Position+m_Direction);

	m_TranslationMatrix.SetIdentity();
	m_TranslationMatrix.SetPos(m_Position.x, m_Position.y, m_Position.z);

	m_TransformMatrix = m_ScaleMatrix*m_RotationMatrix*m_TranslationMatrix;

	return m_TransformMatrix;
}

void CDirectionalLight::SetShadowMap(CRenderManager &RenderManager)
{
	m_ViewShadowMap.SetIdentity();
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position + m_Direction, v3fY);
	unsigned int l_ShadowMapWidth = m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight = m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetFromOrtho(m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, 0.1f, m_EndRangeAttenuation);
	//UABEngine.GetEffectManager()->m_SceneParameters.m_View = m_ViewShadowMap;
	//UABEngine.GetEffectManager()->m_SceneParameters.m_Projection = m_ProjectionShadowMap;
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
