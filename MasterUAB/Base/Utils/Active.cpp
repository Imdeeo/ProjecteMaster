#include "Active.h"
#include "XML\XMLTreeNode.h"

CActive::CActive(const CXMLTreeNode &TreeNode)
{
	m_active = TreeNode.GetBoolProperty("active", false);
}

CActive::CActive(bool l_active)
{
	m_active = l_active;
}