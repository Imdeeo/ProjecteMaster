#ifndef _BOSS_ENEMY
#define _BOSS_ENEMY

#pragma once

#include "Enemies/Enemy.h"

class CBossEnemy : public CEnemy
{
private:
public:
	CBossEnemy(CXMLTreeNode &XMLTreeNode);
	virtual ~CBossEnemy();
	void Update(float ElapsedTime);
	void Render();
	//static CEnemy *FactoryEnemy();
};

#endif
