#ifndef STATIC_MESH_MANAGER_H
#define STATIC_MESH_MANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "StaticMesh\StaticMesh.h"
#include <string>

class CLevel;

class CStaticMeshManager :	public CTemplatedLevelMapManager<CStaticMesh>
{
private:
	std::string m_Filename;
	std::string m_LevelName;
public:
	CStaticMeshManager(void);
	virtual ~CStaticMeshManager(void);
	bool Load(const std::string &FileName, CLevel* _Level);
	bool Reload();
};

#endif // STATIC_MESH_MANAGER_H