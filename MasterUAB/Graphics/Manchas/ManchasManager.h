#ifndef MANCHAS_MANAGER_H
#define MANCHAS_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ManchasSystemType.h"

class CManchasManager : public CTemplatedMapManager<CManchasSystemType>
{
private:
	std::string m_Filename;
public:
	CManchasManager(void);
	virtual ~CManchasManager(void);
	void Load(const std::string &Filename);
	void Reload();
	void Save();
};

#endif //MANCHAS_MANAGER_H