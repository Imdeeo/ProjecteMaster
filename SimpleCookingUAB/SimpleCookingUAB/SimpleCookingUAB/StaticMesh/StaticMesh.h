#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "Base\Utils\Named.h"
#include "Base\Math\Matrix44.h"

#include <vector>

class CRenderableVertexs;
class CRenderManager;

class CStaticMesh :	public CNamed
{
protected:
	std::vector<CRenderableVertexs *> m_RVs;
	unsigned short m_NumFaces;
	long m_NumVertexs;
	long m_NumIndexs;

	std::string m_PhysxMeshesDirectory;
public:
	CStaticMesh();
	virtual ~CStaticMesh();
	bool Load (const std::string &FileName);
	const std::vector<CRenderableVertexs *> &GetRenderableVertexs() const { return m_RVs; }

	void SetPhysxMeshesDirectory(std::string &_PhysxMeshesDirectory){ m_PhysxMeshesDirectory = _PhysxMeshesDirectory; }
	const std::string &GetPhysxMeshesDirectory() const{ return m_PhysxMeshesDirectory; }
	
private:
	bool Destroy ();
};

#endif //STATIC_MESH_H