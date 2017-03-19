#ifndef MANCHAS_MANAGER_H
#define MANCHAS_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "ManchasSystemType.h"

class CLevel;

class CManchasManager : public CTemplatedLevelMapManager<CManchasSystemType>
{
private:
	std::string m_Filename;
	std::string m_LevelName;
public:
	CManchasManager(void);
	virtual ~CManchasManager(void);
	void Load(const std::string &Filename, CLevel* _Level);
	void Reload();
	void Save();
};

#endif //MANCHAS_MANAGER_H