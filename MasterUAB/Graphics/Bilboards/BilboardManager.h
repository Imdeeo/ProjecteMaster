#ifndef BILBOARD_MANAGER_H
#define BILBOARD_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "BilboardSystemType.h"

class CBilboardManager : public CTemplatedMapManager<CBilboardSystemType>
{
private:
	std::string m_Filename;
	std::string m_LevelId;
public:
	CBilboardManager(void);
	virtual ~CBilboardManager(void);
	void Load(const std::string &_Filename, const std::string &_LevelId);
	void Reload();
};

#endif //BILBOARD_MANAGER_H