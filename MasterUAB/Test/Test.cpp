// Test.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Enemies\EnemyManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CEnemyManager l_EnemyManager;
	l_EnemyManager.Load("./data/enemies.xml");

	l_EnemyManager.Update(0.0f);
	l_EnemyManager.Render();
	return 0;
}

