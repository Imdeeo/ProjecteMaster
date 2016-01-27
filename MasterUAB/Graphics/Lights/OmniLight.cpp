#include "OmniLight.h"

#include "XML\XMLTreeNode.h"

COmniLight::COmniLight():CLight(){}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) : CLight(TreeNode){}