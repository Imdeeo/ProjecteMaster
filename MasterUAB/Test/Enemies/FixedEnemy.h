#ifndef FIXED_ENEMY_H
#define FIXED_ENEMY_H

#include "Enemies/Enemy.h"

class CFixedEnemy : public CEnemy
{
private:
public:
	CFixedEnemy(CXMLTreeNode &XMLTreeNode);
	virtual ~CFixedEnemy();
	void Update(float ElapsedTime);
	void Render();
	//static CEnemy *FactoryEnemy();
};

#endif
