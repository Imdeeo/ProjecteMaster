#include "DirectionalLight.h"

#include "XML\XMLTreeNode.h"

CDirectionalLight::CDirectionalLight() : CLight() {}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	SetDirection(TreeNode.GetFloatProperty("dir"));
}

void CDirectionalLight::Render(CRenderManager *RenderManager)
{

}