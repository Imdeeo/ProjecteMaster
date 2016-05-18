#ifndef INSTANCE_MESH_H
#define INSTANCE_MESH_H

#include "RenderableObjects\RenderableObject.h"

class CXMLTreeNode;
class CStaticMesh;

class CInstanceMesh : public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
	bool m_GeneratePhysx;
	std::string m_PxType;
	std::string m_PxMaterial;
	int m_PxGroup;
	float m_PxOffset;
	Vect3f m_PxNormals;
public:
	CInstanceMesh(const CXMLTreeNode &TreeNode);
	CInstanceMesh(const std::string &Name, const std::string &CoreName);
	~CInstanceMesh();
	void Render(CRenderManager *RM);
	void Save(FILE* _File, std::string _layer);

#ifdef _DEBUG
	void RenderDebug(CRenderManager *RM);
#endif

	CInstanceMesh & CInstanceMesh::operator=(CInstanceMesh &);
};

#endif //INSTANCE_MESH_H