#include "DirectionalLight.h"

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
