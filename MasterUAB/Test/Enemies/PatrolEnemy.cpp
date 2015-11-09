#include "PatrolEnemy.h"
#include <stdio.h>

CPatrolEnemy::CPatrolEnemy(CXMLTreeNode &XMLTreeNode)
: CEnemy(XMLTreeNode)
{
	m_Path=XMLTreeNode.GetPszProperty("path", "");
}


CPatrolEnemy::~CPatrolEnemy()
{
}

void CPatrolEnemy::Update(float ElapsedTime)
{
	printf("update patrol enemy\n");
}

void CPatrolEnemy::Render()
{
	printf("render patrol enemy\n");
}

/*CEnemy * CPatrolEnemy::FactoryEnemy()
{
	return new CPatrolEnemy;
}
*/