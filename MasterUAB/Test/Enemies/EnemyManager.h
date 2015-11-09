#ifndef _ENEMY_MANAGER
#define _ENEMY_MANAGER

#pragma once

#include "XML\XMLTreeNode.h"

#include <vector>

class CEnemy;

class CEnemyManager
{
private:
	std::vector<CEnemy *> m_Enemies;
public:
	CEnemyManager();
	virtual ~CEnemyManager();

	void Load(const std::string &Filename);
	void Update(float ElapsedTime);
	void Render();
	void AddEnemy(CXMLTreeNode &XMLTreeNode);
};

#endif
