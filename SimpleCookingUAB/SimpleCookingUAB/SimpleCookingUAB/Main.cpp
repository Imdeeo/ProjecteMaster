#include "StaticMesh\StaticMeshManager.h"
#include "PhyxManager\PhysXManager.h"

void main()
{
	CStaticMeshManager l_StaticMehManager;

	CPhysXManager * l_PhysXManager = CPhysXManager::CreatePhysXManager();

	l_StaticMehManager.Load("Data\\static_meshes.xml");

	char c;
	printf("pene");
	scanf("%c", &c);

}