#ifndef H_ACTIVE_H
#define H_ACTIVE_H

#include "XML\tinyxml2.h"

class CEmptyPointerClass;

class CActive
{
private:
	bool m_Active;
public:
	CActive(tinyxml2::XMLElement* TreeNode);
	CActive(bool l_active);
	virtual ~CActive() {}
	bool GetActive(){ return m_Active; }
	void SetActive(bool _Active){ m_Active = _Active; }

	CEmptyPointerClass* GetLuaAdress();
};

#endif //H_ACTIVE_H