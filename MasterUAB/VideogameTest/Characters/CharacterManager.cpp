#include "CharacterManager.h"

#include "XML\XMLTreeNode.h"

#include "Player.h"


CCharacterManager* CCharacterManager::m_Instance = nullptr;

CCharacterManager* CCharacterManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CCharacterManager();
	}
	return m_Instance;
}

CCharacterManager::CCharacterManager()
{
	
}


CCharacterManager::~CCharacterManager()
{
}


bool CCharacterManager::Load(std::string _Filename)
{
	m_Filename = _Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["characters"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); i++)
			{
				CXMLTreeNode l_Element = l_Input(i);
				if (l_Element.GetName() == std::string("player"))
				{
					std::string l_Name = l_Element.GetPszProperty("name");
					AddResource(l_Name, new CPlayer(l_Element));
					GetResource(l_Name)->ExecuteLuaCommand();
				}
				if (l_Element.GetName() == std::string("enemy"))
				{
				}
			}
		}
	}

	return true;
}

bool CCharacterManager::Reload()
{

	return true;
}