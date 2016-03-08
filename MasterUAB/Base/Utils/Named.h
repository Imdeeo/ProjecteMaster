#ifndef _NAMED_H
#define _NAMED_H


class CXMLTreeNode;

#include <string>

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