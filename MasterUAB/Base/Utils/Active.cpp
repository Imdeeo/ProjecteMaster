#include "Active.h"
#include "CEmptyPointerClass.h"

CActive::CActive(tinyxml2::XMLElement* TreeNode)
{
	m_Active = TreeNode->GetBoolProperty("active", true);
}

CActive::CActive(CXMLTreeNode TreeNode)
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