#ifndef RENDERABLE_VERTEX_H
#define RENDERABLE_VERTEX_H

#include <assert.h>

class CEffectTechnique;
class CRenderManager;

class CRenderableVertexs
{
public:
	virtual const Vect3f* GetVertexs()const
	{
		assert(!"This method mustn't be called");
		return nullptr;
	}
	virtual const unsigned int GetNVertexs()
	{
		assert(!"This method mustn't be called");
		return false;
	}
	virtual const unsigned int GetSizeOfVertex()
	{
		assert(!"This method mustn't be called");
		return false;
	}
	virtual const void* GetIndexs()const
	{
		assert(!"This method mustn't be called");
		return nullptr;
	}
	virtual const unsigned int GetNIndexs()
	{
		assert(!"This method mustn't be called");
		return false;
	}
	virtual const unsigned int GetSizeOfIndexs()
	{
		assert(!"This method mustn't be called");
		return false;
	}
};


#endif //RENDERABLE_VERTEX_H

