#ifndef H_ACTIVE_H
#define H_ACTIVE_H

#include "Utils.h"

class CXMLTreeNode;

class CActive
{
public:
	CActive(const CXMLTreeNode &TreeNode);
	CActive(bool l_active);
	virtual ~CActive() {}
	UAB_BUILD_GET_SET(bool, active);
};

#endif //H_ACTIVE_H