#ifndef INSTANCE_MESH_H
#define INSTANCE_MESH_H

#include "RenderableObject.h"

class CXMLTreeNode;
class CStaticMesh;

class CInstanceMesh :	public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
public:
	CInstanceMesh(const CXMLTreeNode &TreeNode);
	CInstanceMesh(const std::string &Name, const std::string &CoreName);
	~CInstanceMesh();
	void Render(CRenderManager *RM);
};

#endif //INSTANCE_MESH_H