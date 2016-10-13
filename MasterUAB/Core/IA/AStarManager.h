#ifndef ASTAR_MANAGER_H
#define ASTAR_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "AStar.h"

#include <string>

class CAStarManager : public CTemplatedMapManager<CAStar>
{
private:
	std::string m_Filename;
	std::string m_LevelId;
public:
	CAStarManager(void);
	virtual ~CAStarManager(void);
	bool Load(const std::string &FileName, const std::string &_LevelId);
	bool Reload();
};

#endif // ASTAR_MANAGER_H