#ifndef INSTANCE_MESH_H
#define INSTANCE_MESH_H

#include "RenderableObjects\RenderableObject.h"
#include "XML\tinyxml2.h"

class CStaticMesh;
class CFrustum;

class CInstanceMesh : public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
	CFrustum *m_Frustum;
	bool m_GeneratePhysx;
	std::string m_PxType;
	std::string m_PxMaterial;
	std::string m_PxGroup;
	std::string m_Layer;
	float m_PxOffset;
	Vect3f m_PxNormals;
public:
	CInstanceMesh(tinyxml2::XMLElement* TreeNode, const std::string & _LevelId = "");
	CInstanceMesh(const std::string &Name, const std::string &CoreName, const std::string & _LevelId = "");
	~CInstanceMesh();
	void Render(CRenderManager *RM);
	void Save(FILE* _File, std::string _layer);
	bool GetInsideFrustum();

#ifdef _DEBUG
	void RenderDebug(CRenderManager *RM);
#endif

	CInstanceMesh & CInstanceMesh::operator=(CInstanceMesh &);
};

#endif //INSTANCE_MESH_H