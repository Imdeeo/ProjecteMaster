#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "Named.h"
#include <vector>

class CRenderManager;

class CStaticMesh :	public CNamed
{
protected:
	std::vector<CRenderableVertexs *> m_RVs;
	std::vector<CMaterial *> m_Materials;
	unsigned short m_NumFaces;
	long m_NumVertexs;
	long m_NumIndexs;
public:
	CStaticMesh();
	virtual ~CStaticMesh();
	bool Load (const std::string &FileName);
	bool Reload ();
	void Render (CRenderManager *RM) const;
private:
	bool Destroy ();
};

#endif //STATIC_MESH_H