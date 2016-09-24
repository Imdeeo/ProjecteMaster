#ifndef MANCHAS_MANAGER_H
#define MANCHAS_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ManchasSystemType.h"

class CManchasManager : public CTemplatedLevelMapManager<CManchasSystemType>
{
private:
	std::string m_Filename;
public:
	CManchasManager(void);
	virtual ~CManchasManager(void);
	void Load(const std::string &Filename, const std::string &_LevelId);
	void Reload();
	void Save();
};

#endif //MANCHAS_MANAGER_H