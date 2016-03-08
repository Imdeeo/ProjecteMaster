#ifndef CCHARACER_MANAGER_H
#define CCHARACER_MANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Character.h"
class CXMLTreeNode;

class CCharacterManager : public CTemplatedVectorMapManager<CCharacter>
{
private:
	static CCharacterManager* m_Instance;
	std::string m_Filename;

	CCharacterManager();
public:
	static CCharacterManager* GetInstance();
	virtual ~CCharacterManager();

	bool Load(const std::string filename);
	bool Reload();
};

#endif