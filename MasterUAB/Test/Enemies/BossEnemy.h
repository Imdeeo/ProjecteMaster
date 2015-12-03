#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

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