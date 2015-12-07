#include "DirectionalLight.h"

#include "XML\XMLTreeNode.h"

CDirectionalLight::CDirectionalLight() : CLight() {}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	m_Direction = TreeNode.GetFloatProperty("dir");
}

void CDirectionalLight::Render(CRenderManager *RenderManager)
{

}