#include "BossEnemy.h"
#include <stdio.h>

CBossEnemy::CBossEnemy(CXMLTreeNode &XMLTreeNode)
: CEnemy(XMLTreeNode)
{
}

CBossEnemy::~CBossEnemy()
{
}

void CBossEnemy::Update(float ElapsedTime)
{
	printf("update boss enemy\n");
}

void CBossEnemy::Render()
{
	printf("render boss enemy\n");
}

/*CEnemy * CBossEnemy::FactoryEnemy()
{
	return new CBossEnemy;
}*/
