#include "Active.h"
#include "XML\XMLTreeNode.h"
#include "CEmptyPointerClass.h"

CActive::CActive(const CXMLTreeNode &TreeNode)
{
	m_Active = TreeNode.GetBoolProperty("active", true);
}

CActive::CActive(bool l_active)
{
	m_Active = l_active;
}

CEmptyPointerClass* CActive::GetLuaAdress()
{
	return (CEmptyPointerClass*)&m_Active;
}