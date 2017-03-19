#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include "Base\Utils\TemplatedMapManager.h"
#include "StaticMesh\StaticMesh.h"
#include <string>

class CPhysXManager;

class CStaticMeshManager :	public CTemplatedMapManager<CStaticMesh>
{
private:
	std::string m_DirName;
public:
	CStaticMeshManager(void);
	virtual ~CStaticMeshManager(void);
	bool Load(const std::string &_DirName,const std::string &FileName);

	void CreatePhysXMeshes(CPhysXManager*);
};

#endif // STATIC_MESH_MANAGER_H