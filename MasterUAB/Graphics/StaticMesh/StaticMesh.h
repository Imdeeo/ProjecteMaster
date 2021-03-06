#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "Utils\Named.h"
#include "Math\Matrix44.h"
#include "Materials\Material.h"

#include "Utils\LevelInfo.h"

class CRenderableVertexs;
class CRenderManager;
class CLevel;

class CStaticMesh :	public CNamed, public CLevelInfo
{
protected:
	std::vector<CRenderableVertexs *> m_RVs;
	std::vector<CMaterial *> m_Materials;
	unsigned short m_NumFaces;
	long m_NumVertexs;
	long m_NumIndexs;
	Vect3f m_BoundingBoxMax;
	Vect3f m_BoundingBoxMin;
	Vect3f m_BoundingSphereCenter;
	float  m_BoundingSphereRadius;
	std::string m_FileName;

	std::string m_PhysxMeshesDirectory;
public:
	CStaticMesh(CLevel* _Level);
	virtual ~CStaticMesh();
	bool Load(const std::string &FileName, CLevel* _Level);
	bool Reload ();
	void Render (CRenderManager *RM) const;
	void CStaticMesh::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t
		VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t
		NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

	Vect3f GetBoundingBoxMax(){ return m_BoundingBoxMax; }
	Vect3f GetBoundingBoxMin(){ return m_BoundingBoxMin; }
	Vect3f GetBoundingSphereCenter(){ return m_BoundingSphereCenter; }
	float GetBoundingSphereRadius(){ return m_BoundingSphereRadius; }

	void SetPhysxMeshesDirectory(std::string &_PhysxMeshesDirectory){ m_PhysxMeshesDirectory = _PhysxMeshesDirectory; }
	const std::string &GetPhysxMeshesDirectory() const{ return m_PhysxMeshesDirectory; }

	const std::vector<CRenderableVertexs *> &GetRenderableVertexs() const { return m_RVs; }

	std::vector<CMaterial *> GetMaterials()const;
	void SetMaterials(std::vector<CMaterial *>);
	
private:
	bool Destroy ();
};

#endif //STATIC_MESH_H