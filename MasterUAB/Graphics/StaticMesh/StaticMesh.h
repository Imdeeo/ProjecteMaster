#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "Utils.h"
#include "Utils\Named.h"
#include "Math\MathTypes.h"

#include <assert.h>

class CRenderableVertexs;
class CMaterial;
class CRenderManager;

class CStaticMesh :	public CNamed
{
protected:
	std::vector<CRenderableVertexs *> m_RVs;
	std::vector<CMaterial *> m_Materials;
	unsigned short m_NumFaces;
	long m_NumVertexs;
	long m_NumIndexs;

	Vect3f* m_BoundingBoxMax;
	Vect3f* m_BoundingBoxMin;
	Vect3f* m_BoundingSphereCenter;
	float  m_BoundingSphereRadius;

public:
	CStaticMesh();
	virtual ~CStaticMesh();
	bool Load (const std::string &FileName);
	bool Reload ();
	void Render (CRenderManager *RM) const;
	void CStaticMesh::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t
		VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t
		NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);
	
private:
	bool Destroy ();

	UAB_GET_PROPERTY(Vect3f*,BoundingBoxMax);
	UAB_GET_PROPERTY(Vect3f*,BoundingBoxMin);
	UAB_GET_PROPERTY(Vect3f*,BoundingSphereCenter);
	UAB_GET_PROPERTY(float  ,BoundingSphereRadius);
	UAB_GET_PROPERTY(std::vector<CMaterial *>, Materials);
};

#endif //STATIC_MESH_H