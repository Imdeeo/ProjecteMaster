#include "XMLTreeNode.h"

#define MY_ENCODING "ISO-8859-1"

CXMLTreeNode::CXMLTreeNode()
{
	m_xmlDoc = new tinyxml2::XMLDocument();
}

CXMLTreeNode::~CXMLTreeNode()
{	
}

bool CXMLTreeNode::LoadFile (const char* _pszFileName)
{
	tinyxml2::XMLError eResult = m_xmlDoc->LoadFile(_pszFileName);

	if (eResult == tinyxml2::XML_SUCCESS)
	{
		m_pNode = m_xmlDoc->RootElement();
		if (m_pNode == nullptr)
			return false;
		else
			return true;
	}
	else
		return false;
}

CXMLTreeNode CXMLTreeNode::GetSubTree(const char* _pszKey) const
{
	assert(m_pNode && _pszKey);
	CXMLTreeNode NewTree;

	if (_pszKey && m_pNode)
	{
		_FindSubTree(m_pNode, _pszKey, NewTree);
	}

	return NewTree;
}

bool CXMLTreeNode::_FindSubTree(tinyxml2::XMLElement* _pNode, const char* _pszKey, CXMLTreeNode& _TreeFound) const
{
	while (_pNode != NULL)
	{
		if (strcmp(_pNode->Name(), _pszKey) != 0)
		{
			if (_FindSubTree(_pNode->FirstChildElement(), _pszKey, _TreeFound))
			{
				return true;
			}
		}
		else
		{
			_TreeFound.m_pNode = _pNode;
			return true;
		}

		_pNode = _pNode->NextSiblingElement();
	}

	return false;
}

bool CXMLTreeNode::ExistsKey(const char* _pszKey)
{
	assert(_pszKey);

	CXMLTreeNode TreeFound = GetSubTree(_pszKey);
	return TreeFound.Exists();
}

//----------------------------------------------------------------------------
// Returns the main node keyword
//----------------------------------------------------------------------------
const char* CXMLTreeNode::GetName ()
{
	if (m_pNode)
	{
		return (char*)m_pNode->Name();
	}

	return NULL;
}

//----------------------------------------------------------------------------
// Operator that returns a tree node
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator[] (const char* _pszKey) const
{
  assert(_pszKey && m_pNode);

  CXMLTreeNode TreeFound;

  if (_pszKey && m_pNode)
  {
    TreeFound = GetSubTree(_pszKey);
  }

  return TreeFound;
}

//----------------------------------------------------------------------------
// Operator to get children nodes
//----------------------------------------------------------------------------
CXMLTreeNode CXMLTreeNode::operator() (int _iIndex) const
{
	assert(_iIndex >= 0 && m_pNode);

	CXMLTreeNode TreeFound;

	if (_iIndex >= 0 && m_pNode)
	{
		int iCount = 0;
		tinyxml2::XMLElement* pChildren = m_pNode->FirstChildElement();
		while (pChildren != NULL)
		{		  
			if (_iIndex == iCount)
			{
				TreeFound.m_pNode = pChildren;
				break;
			}

			++iCount;
			pChildren = pChildren->NextSiblingElement();
		}
	}
	
	return TreeFound;
}

//----------------------------------------------------------------------------
// Returns the number of children a tree has
//----------------------------------------------------------------------------
int CXMLTreeNode::GetNumChildren ()  
{
	assert(m_pNode);

	int iCount = 0;

	if (m_pNode && !m_pNode->NoChildren())
	{		
		tinyxml2::XMLElement* pChildren = m_pNode->FirstChildElement();
		while (pChildren != NULL)
		{
			++iCount;
			pChildren = pChildren->NextSiblingElement();
		}
	}

	return iCount;
}

//----------------------------------------------------------------------------
// Returns an integer param if found. Else a default value
//----------------------------------------------------------------------------
int CXMLTreeNode::GetIntProperty (const char* _pszKey, int _iDefault, bool warningDefault) const
{
	int iRet;

	tinyxml2::XMLError eResult = m_pNode->QueryIntAttribute(_pszKey, &iRet);

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		iRet = _iDefault;
	}

	return iRet;
}

//----------------------------------------------------------------------------
// Returns a float param if found. Else a default value
//----------------------------------------------------------------------------
float CXMLTreeNode::GetFloatProperty (const char* _pszKey, float _fDefault, bool warningDefault) const
{
	float fRet;

	tinyxml2::XMLError eResult = m_pNode->QueryFloatAttribute(_pszKey, &fRet);

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		fRet = _fDefault;
	}

	return fRet;
}
//----------------------------------------------------------------------------
// Returns a boolean param if found. Else a default value
//----------------------------------------------------------------------------

bool CXMLTreeNode::GetBoolProperty (const char* _pszKey, bool _bDefault, bool warningDefault) const
{
	bool bRet;

	tinyxml2::XMLError eResult = m_pNode->QueryBoolAttribute(_pszKey, &bRet);

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		bRet = _bDefault;
	}

	return bRet;
}

//----------------------------------------------------------------------------
// Returns an string param if found. Else a default value
//----------------------------------------------------------------------------
const char* CXMLTreeNode::GetPszProperty (const char* _pszKey, const char* _pszDefault, bool warningDefault) const
{
	const char* pszRet = nullptr;

	pszRet = m_pNode->Attribute(_pszKey);
	if (pszRet == nullptr) 
		pszRet = _pszDefault;
	
	return pszRet;
}

//----------------------------------------------------------------------------
// Returns an Vect2f param if found. Else a default value
//----------------------------------------------------------------------------
Vect2f CXMLTreeNode::GetVect2fProperty  (const char* _pszKey, const Vect2f& _Default, bool warningDefault) const
{
  Vect2f l_V2f(0.0f, 0.0f);
  const char* pszRet = nullptr;

  pszRet = m_pNode->Attribute(_pszKey);
  if (pszRet == nullptr)
	  l_V2f = _Default;
  else
	  sscanf_s(pszRet, "%f %f", &l_V2f.x, &l_V2f.y);
  
  return l_V2f;
}

//----------------------------------------------------------------------------
// Returns an Vect3f param if found. Else a default value
//----------------------------------------------------------------------------
Vect3f CXMLTreeNode::GetVect3fProperty  (const char* _pszKey, const Vect3f& _Default, bool warningDefault) const
{
	Vect3f l_V3f(0.0f, 0.0f, 0.0f);
	const char* pszRet = nullptr;

	pszRet = m_pNode->Attribute(_pszKey);
	if (pszRet == nullptr)
		l_V3f = _Default;
	else
		sscanf_s(pszRet, "%f %f %f", &l_V3f.x, &l_V3f.y, &l_V3f.z);

	return l_V3f;
}

//----------------------------------------------------------------------------
// Returns an Vect4f param if found. Else a default value
//----------------------------------------------------------------------------
Vect4f CXMLTreeNode::GetVect4fProperty  (const char* _pszKey, const Vect4f& _Default, bool warningDefault) const
{
	Vect4f l_V4f(0.0f, 0.0f, 0.0f, 0.0f);
	const char* pszRet = nullptr;

	pszRet = m_pNode->Attribute(_pszKey);
	if (pszRet == nullptr)
		l_V4f = _Default;
	else
		sscanf_s(pszRet, "%f %f %f %f", &l_V4f.x, &l_V4f.y, &l_V4f.z, &l_V4f.w);

	return l_V4f;
}

//----------------------------------------------------------------------------
// Returns a Quatf param if found. Else a default value
//----------------------------------------------------------------------------
Quatf CXMLTreeNode::GetQuatfProperty(const char* _pszKey, const Quatf& _Default, bool warningDefault) const
{
	Quatf l_Qf(0.0f, 0.0f, 0.0f, 0.0f);
	const char* pszRet = nullptr;

	pszRet = m_pNode->Attribute(_pszKey);
	if (pszRet == nullptr)
		l_Qf = _Default;
	else
		sscanf_s(pszRet, "%f %f %f %f", &l_Qf.x, &l_Qf.y, &l_Qf.z, &l_Qf.w);

	return l_Qf;
}

/*
//----------------------------------------------------------------------------
// Returns an Vect2i param if found. Else a default value
//----------------------------------------------------------------------------
Vect2i CXMLTreeNode::GetVect2iProperty  (const char* _pszKey, const Vect2i& _Default, bool warningDefault) const
{
  xmlChar* value = GetProperty(_pszKey);
  Vect2i l_V2i(0, 0);

  if (value)
  {
    const char* pszValue = (const char*)value;
    sscanf_s(pszValue,"%d %d",&l_V2i.x, &l_V2i.y);    
  }
	else if(warningDefault)
	{
		//LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector2i por defecto:(%d,%d) para el tag <%s>", _Default.x, _Default.y, _pszKey);
	}

  xmlFree(value);
  return l_V2i;
}

//----------------------------------------------------------------------------
// Returns an Vect3i param if found. Else a default value
//----------------------------------------------------------------------------
Vect3i CXMLTreeNode::GetVect3iProperty  (const char* _pszKey, const Vect3i& _Default, bool warningDefault) const
{
  xmlChar* value = GetProperty(_pszKey);
  Vect3i l_V3i(0, 0, 0);

  if (value)
  {
    const char* pszValue = (const char*)value;
    sscanf_s(pszValue,"%d %d %d",&l_V3i.x, &l_V3i.y, &l_V3i.z);    
  }
	else if(warningDefault)
	{
		//LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector3i por defecto:(%d,%d,%d) para el tag <%s>",_Default.x,_Default.y,_Default.z, _pszKey);
	}

  xmlFree(value);
  return l_V3i;
}

//----------------------------------------------------------------------------
// Returns an Vect4i param if found. Else a default value
//----------------------------------------------------------------------------
Vect4i CXMLTreeNode::GetVect4iProperty  (const char* _pszKey, const Vect4i& _Default, bool warningDefault) const
{
  xmlChar* value = GetProperty(_pszKey);
  Vect4i l_V4i(0, 0, 0, 0);

  if (value)
  {
    const char* pszValue = (const char*)value;
    sscanf_s(pszValue,"%d %d %d %d",&l_V4i.x, &l_V4i.y, &l_V4i.z, &l_V4i.w);    
  }
	else if(warningDefault)
	{
		//LOGGER->AddNewLog(ELL_WARNING, "CXMLTreeNode::GetVect3fProperty se ha utilizado el vector4i por defecto:(%d,%d,%d,%d) para el tag <%s>", _Default.x, _Default.y, _Default.z, _Default.w, _pszKey);
	}

  xmlFree(value);
  return l_V4i;
}
*/