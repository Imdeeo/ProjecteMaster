#ifndef _NAMED_H
#define _NAMED_H

#include <string>

class CXMLTreeNode;

class CNamed
{
protected:
	std::string m_Name;
public:
	CNamed(const CXMLTreeNode &TreeNode);
	CNamed(const std::string &Name);
	virtual void SetName(const std::string &Name);
	virtual const std::string &GetName();
};

#endif //_NAMED_H