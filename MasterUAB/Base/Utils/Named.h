#ifndef _NAMED_H
#define _NAMED_H

#include "XML\tinyxml2.h"
#include <string>
#include "no_sillywarnings_please.h"

class CEmptyPointerClass;

class CNamed
{
protected:
	std::string m_Name;
public:
	CNamed(tinyxml2::XMLElement* TreeNode);
	CNamed(const std::string &Name);
	virtual void SetName(const std::string &Name);
	virtual const std::string &GetName();

	CEmptyPointerClass* GetLuaNameAddress();
};

#endif //_NAMED_H