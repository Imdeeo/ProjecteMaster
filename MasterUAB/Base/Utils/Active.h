#ifndef H_ACTIVE_H
#define H_ACTIVE_H

class CXMLTreeNode;
class CEmptyPointerClass;

class CActive
{
private:
	bool m_Active;
public:
	CActive(const CXMLTreeNode &TreeNode);
	CActive(bool l_active);
	virtual ~CActive() {}
	bool GetActive(){ return m_Active; }
	void SetActive(bool _Active){ m_Active = _Active; }

	CEmptyPointerClass* GetLuaAdress();
};

#endif //H_ACTIVE_H