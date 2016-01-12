#include "DirectionalLight.h"

#include "XML\XMLTreeNode.h"

CDirectionalLight::CDirectionalLight() : CLight(),m_Direction(Vect3f(0.0f,0.0f,0.0f)) {}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	m_Direction = TreeNode.GetVect3fProperty("dir",Vect3f(0.0,0.0,0.0));
}

void CDirectionalLight::Render(CRenderManager *RenderManager)
{

}