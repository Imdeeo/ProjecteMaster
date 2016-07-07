#ifndef XMLTREENODE_H
#define XMLTREENODE_H

// Includes
#include "tinyxml2.h"
#include <string>
#include "Math/Vector4.h"
#include "Math/Quatn.h"

//Class
class CXMLTreeNode
{
public:
	// Init and End protocols
	CXMLTreeNode();
	virtual ~CXMLTreeNode();

	// -----------------------
	// Read functions
	// -----------------------
	bool LoadFile(const char* _pszFileName);
	bool Exists() { return m_pNode != NULL; }
	bool ExistsKey(const char* _pszKey);
	const char*	GetName();

	// To get properties from xml file
	int	GetIntProperty(const char* _pszKey, int _iDefault=0, bool warningDefault = true) const;
	float GetFloatProperty(const char* _pszKey, float _fDefault=0.0, bool warningDefault = true) const;
	bool GetBoolProperty(const char* _pszKey, bool _bDefault=false, bool warningDefault = true) const;
	const char*	GetPszProperty(const char* _pszKey, const char* _pszDefault=NULL, bool warningDefault = true) const;
	Vect2f GetVect2fProperty(const char* _pszKey, const Vect2f& _Default, bool warningDefault = true) const;
	Vect3f GetVect3fProperty(const char* _pszKey, const Vect3f& _Default, bool warningDefault = true) const;
	Vect4f GetVect4fProperty(const char* _pszKey, const Vect4f& _Default, bool warningDefault = true) const;
	Quatf GetQuatfProperty(const char* _pszKey, const Quatf& _Default, bool warningDefault = true) const;
	int	GetNumChildren();

	CXMLTreeNode operator[](const char* _pszKey) const;
	CXMLTreeNode operator()(int _iIndex) const;

	bool IsComment() const;
private:	
	tinyxml2::XMLElement* m_pNode;

	CXMLTreeNode GetSubTree(const char* _pszKey) const;
};
#endif