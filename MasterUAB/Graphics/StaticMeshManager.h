#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include "TemplatedMapManager.h"
#include "StaticMesh.h"
#include <string>

class CStaticMeshManager :	public CTemplatedMapManager<CStaticMesh>
{
private:
	std::string m_Filename;
public:
	CStaticMeshManager(void);
	virtual ~CStaticMeshManager(void);
	bool Load(const std::string &FileName);
	bool Reload();
};

#endif // STATIC_MESH_MANAGER_H