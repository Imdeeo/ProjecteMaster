#ifndef RENDERABLE_VERTEX_H
#define RENDERABLE_VERTEX_H

#include "RenderManager.h"
#include <assert.h>

class CEffectTechnique;

class CRenderableVertexs
{
public:
	virtual bool Render(CRenderManager *RenderManager, CEffectTechnique	*EffectTechnique, void *Parameters)
	{
		assert(!"This method mustn't be called");
		return false;
	}
	virtual bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique,
			void *Parameters, unsigned int IndexCount=-1, unsigned int StartIndexLocation=0,
			unsigned int BaseVertexLocation=0)
	{
		assert(!"This method mustn't be called");
		return false;
	}
};


#endif //RENDERABLE_VERTEX_H


