#ifndef PATROL_ENEMY_H
#define PATROL_ENEMY_H

#include "Enemy.h"
#include <string>

class CPatrolEnemy : public CEnemy
{
private:
	std::string m_WaypointTrack;
	std::string m_Path;
public:
	CPatrolEnemy(CXMLTreeNode &XMLTreeNode);
	virtual ~CPatrolEnemy();
	void Update(float ElapsedTime);
	void Render();
	//static CEnemy *FactoryEnemy();
};

#endif
