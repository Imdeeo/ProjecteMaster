#ifndef _NAMED_H
#define _NAMED_H


class CXMLTreeNode;
class CEmptyPointerClass;

#include <string>
#include "Base\no_sillywarnings_please.h"

class CNamed
{
protected:
	std::string m_Name;
public:
	CNamed(const CXMLTreeNode &TreeNode);
	CNamed(const std::string &Name);
	virtual void SetName(const std::string &Name);
	virtual const std::string &GetName();

	CEmptyPointerClass* GetLuaNameAddress();
};

#endif //_NAMED_H