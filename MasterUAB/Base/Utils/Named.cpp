#include "Utils\Named.h"
#include "CEmptyPointerClass.h"


CNamed::CNamed(tinyxml2::XMLElement* TreeNode)
{
	m_Name = TreeNode->GetPszProperty("name", "");
}

CNamed::CNamed(const std::string &Name):m_Name(Name)
{
}

void CNamed::SetName(const std::string &Name)
{
	m_Name = Name;
}
const std::string& CNamed::GetName()
{
	return m_Name;
}

CEmptyPointerClass* CNamed::GetLuaNameAddress()
{
	return (CEmptyPointerClass*)&m_Name.c_str()[0];
}