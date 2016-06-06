#include "StaticMesh\StaticMeshManager.h"
#include "PhyxManager\PhysXManager.h"

void main()
{
	CStaticMeshManager l_StaticMeshManager;

	CPhysXManager * l_PhysXManager = CPhysXManager::CreatePhysXManager();
	//HEY LISTEN:::: S'ha d'implementar una manera en la que li diguis una carpeta fagi tot a aquella carpeta.
	l_StaticMeshManager.Load("Data\\level2\\static_meshes.xml");

	l_StaticMeshManager.CreatePhysXMeshes(l_PhysXManager);

	char c;
	printf("pene");
	scanf("%c", &c);

}