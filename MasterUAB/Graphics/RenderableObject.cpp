#include "RenderableObject.h"


void CRenderableObject::AddSubmesh(const CRenderableVertexs* _Vertices, const std::string& _Material, float _BoundingRadius, const Vect3f& _BoundingBoxMin, const Vect3f& _BoundingBoxMax)
{
	SSubmesh l_Submesh = { _Vertices, _Material };
	m_Submeshes.push_back(l_Submesh);
	if (GetNumSubmeshes() == 0 || m_BoundingRadius < _BoundingRadius) m_BoundingRadius = _BoundingRadius;

	if (GetNumSubmeshes() == 0 || m_BoundingBoxMin.x > _BoundingBoxMin.x) m_BoundingBoxMin.x = _BoundingBoxMin.x;
	if (GetNumSubmeshes() == 0 || m_BoundingBoxMin.y > _BoundingBoxMin.y) m_BoundingBoxMin.y = _BoundingBoxMin.y;
	if (GetNumSubmeshes() == 0 || m_BoundingBoxMin.z > _BoundingBoxMin.z) m_BoundingBoxMin.z = _BoundingBoxMin.z;

	if (GetNumSubmeshes() == 0 || m_BoundingBoxMax.x < _BoundingBoxMax.x) m_BoundingBoxMax.x = _BoundingBoxMax.x;
	if (GetNumSubmeshes() == 0 || m_BoundingBoxMax.y < _BoundingBoxMax.y) m_BoundingBoxMax.y = _BoundingBoxMax.y;
	if (GetNumSubmeshes() == 0 || m_BoundingBoxMax.z < _BoundingBoxMax.z) m_BoundingBoxMax.z = _BoundingBoxMax.z;
}
