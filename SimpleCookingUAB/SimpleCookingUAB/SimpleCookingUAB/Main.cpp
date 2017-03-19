#include "StaticMesh\StaticMeshManager.h"
#include "PhyxManager\PhysXManager.h"

void main()
{
	CStaticMeshManager l_StaticMeshManager;

	CPhysXManager * l_PhysXManager = CPhysXManager::CreatePhysXManager();
	//HEY LISTEN:::: S'ha d'implementar una manera en la que li diguis una carpeta fagi tot a aquella carpeta.
	char l_Directory[256];
	printf_s("Directory: ");
	wscanf_s(L"%255S", l_Directory,_countof(l_Directory));
	fflush(stdin);
	printf_s("\n");
	if(l_StaticMeshManager.Load(l_Directory,"static_meshes.xml"))
		l_StaticMeshManager.CreatePhysXMeshes(l_PhysXManager);

	char c;
	printf("pene");
	scanf_s("%c", &c);

}