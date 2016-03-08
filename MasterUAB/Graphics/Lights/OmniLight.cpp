#include "OmniLight.h"

#include <assert.h>
#include "XML\XMLTreeNode.h"

COmniLight::COmniLight():CLight(){}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) : CLight(TreeNode){}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{
	assert(!"this method must not be called");
}