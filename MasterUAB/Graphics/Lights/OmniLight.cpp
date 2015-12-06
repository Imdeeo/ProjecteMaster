#include "OmniLight.h"

#include "XML\XMLTreeNode.h"

COmniLight::COmniLight(){}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) : CLight(TreeNode){}