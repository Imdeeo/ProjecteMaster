#include "FixedEnemy.h"
#include <stdio.h>

CFixedEnemy::CFixedEnemy(CXMLTreeNode &XMLTreeNode)
: CEnemy(XMLTreeNode)
{
}

CFixedEnemy::~CFixedEnemy()
{
}

void CFixedEnemy::Update(float ElapsedTime)
{
	printf("update fixed enemy\n");
}

void CFixedEnemy::Render()
{
	printf("render fixed enemy\n");
}

/*CEnemy * CFixedEnemy::FactoryEnemy()
{
	return new CFixedEnemy;
}
*/