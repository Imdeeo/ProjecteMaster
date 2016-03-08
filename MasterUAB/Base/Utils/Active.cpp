#include "Active.h"
#include "XML\XMLTreeNode.h"

CActive::CActive(const CXMLTreeNode &TreeNode)
{
	m_Active = TreeNode.GetBoolProperty("active", false);
}

CActive::CActive(bool l_active)
{
	m_Active = l_active;
}