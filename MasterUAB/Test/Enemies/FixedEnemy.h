#ifndef _FIXED_ENEMY
#define _FIXED_ENEMY

#pragma once

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
