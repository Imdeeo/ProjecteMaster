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

	void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t
		VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t
		NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
	{
		Vect3f *tan1 = new Vect3f[VtxCount * 2];
		Vect3f *tan2 = tan1 + VtxCount;
		ZeroMemory(tan1, VtxCount * sizeof(Vect3f) * 2);
		unsigned char *l_VtxAddress = (unsigned char *)VtxsData;

		for (size_t b = 0; b<IdxCount; b += 3)
		{
			unsigned short i1 = IdxsData[b];
			unsigned short i2 = IdxsData[b + 1];
			unsigned short i3 = IdxsData[b + 2];
			Vect3f *v1 = (Vect3f *)&l_VtxAddress[i1*VertexStride + GeometryStride];
			Vect3f *v2 = (Vect3f *)&l_VtxAddress[i2*VertexStride + GeometryStride];
			Vect3f *v3 = (Vect3f *)&l_VtxAddress[i3*VertexStride + GeometryStride];
			Vect2f *w1 = (Vect2f *)&l_VtxAddress[i1*VertexStride + TextureCoordsStride];
			Vect2f *w2 = (Vect2f *)&l_VtxAddress[i2*VertexStride + TextureCoordsStride];
			Vect2f *w3 = (Vect2f *)&l_VtxAddress[i3*VertexStride + TextureCoordsStride];
			float x1 = v2->x - v1->x;
			float x2 = v3->x - v1->x;
			float y1 = v2->y - v1->y;
			float y2 = v3->y - v1->y;
			float z1 = v2->z - v1->z;
			float z2 = v3->z - v1->z;
			float s1 = w2->x - w1->x;
			float s2 = w3->x - w1->x;
			float t1 = w2->y - w1->y;
			float t2 = w3->y - w1->y;
			float r = 1.0F / (s1 * t2 - s2 * t1);
			Vect3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			Vect3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
			assert(i1<VtxCount);
			assert(i2<VtxCount);
			assert(i3<VtxCount);
			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;
			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (size_t b = 0; b<VtxCount; ++b)
		{
			Vect3f *l_NormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + NormalStride];
			Vect3f *l_TangentVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + TangentStride];
			Vect4f *l_TangentVtx4 = (Vect4f *)&l_VtxAddress[b*VertexStride + TangentStride];
			Vect3f *l_BiNormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + BiNormalStride];
			const Vect3f& t = tan1[b];
			// Gram-Schmidt orthogonalize
			Vect3f l_VAl = t - (*l_NormalVtx)*((*l_NormalVtx)*t);
			l_VAl.Normalize();
			*l_TangentVtx = l_VAl;
			// Calculate handedness
			Vect3f l_Cross;
			l_Cross = (*l_NormalVtx) ^ (*l_TangentVtx);
			l_TangentVtx4->w = (l_Cross*(tan2[b]))< 0.0f ? -1.0f : 1.0f;
			*l_BiNormalVtx = (*l_NormalVtx) ^ (*l_TangentVtx);
		}
		delete[] tan1;
	}
private:
	bool Destroy ();

	UAB_GET_PROPERTY(Vect3f*,BoundingBoxMax);
	UAB_GET_PROPERTY(Vect3f*,BoundingBoxMin);
	UAB_GET_PROPERTY(Vect3f*,BoundingSphereCenter);
	UAB_GET_PROPERTY(float  ,BoundingSphereRadius);
};

#endif //STATIC_MESH_H