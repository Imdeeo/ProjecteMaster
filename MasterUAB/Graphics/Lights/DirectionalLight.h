#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CDirectionalLight : public CLight
{
protected:
	Vect2f m_OrthoShadowMapSize;
	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);
public:
	CDirectionalLight();
	CDirectionalLight(CXMLTreeNode &TreeNode);
	UAB_BUILD_GET_SET_BY_REFERENCE(Vect3f, Direction);
	virtual void Render(CRenderManager *RenderManager);
	virtual const Mat44f & GetTransform();
	void SetShadowMap(CRenderManager &RenderManager);
};

#endif //DIRECTIONAL_LIGHT_H