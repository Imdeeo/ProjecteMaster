#ifndef BILBOARD_MANAGER_H
#define BILBOARD_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "BilboardSystemType.h"

class CLevel;

class CBilboardManager : public CTemplatedMapManager<CBilboardSystemType>
{
private:
	std::string m_Filename;
	std::string m_LevelId;
public:
	CBilboardManager(void);
	virtual ~CBilboardManager(void);
	void Load(const std::string &_Filename, CLevel* _Level);
	void Reload();
};

#endif //BILBOARD_MANAGER_H