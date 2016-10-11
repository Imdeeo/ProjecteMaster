#ifndef BILBOARD_MANAGER_H
#define BILBOARD_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "BilboardSystemType.h"

class CBilboardManager : public CTemplatedMapManager<CBilboardSystemType>
{
private:
	std::string m_Filename;
public:
	CBilboardManager(void);
	virtual ~CBilboardManager(void);
	void Load(const std::string &Filename);
	void Reload();
};

#endif //BILBOARD_MANAGER_H