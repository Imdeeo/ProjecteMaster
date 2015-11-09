#ifndef _PATROL_ENEMY
#define _PATROL_ENEMY

#pragma once

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
