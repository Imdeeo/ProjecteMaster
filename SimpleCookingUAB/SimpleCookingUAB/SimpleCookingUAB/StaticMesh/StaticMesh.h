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
public:
	CStaticMesh();
	virtual ~CStaticMesh();
	bool Load (const std::string &FileName);
	const std::vector<CRenderableVertexs *> &GetRenderableVertexs() const { return m_RVs; }
	
private:
	bool Destroy ();
};

#endif //STATIC_MESH_H