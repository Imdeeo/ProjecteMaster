#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CDirectionalLight : public CLight
{
protected:
	Vect2f m_OrthoShadowMapSize;
	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);

	Vect3f m_Direction;
public:
	CDirectionalLight();
	CDirectionalLight(CXMLTreeNode &TreeNode);

	const Vect3f & GetDirection() const { return m_Direction; }
	void SetDirection(Vect3f _Direction) { m_Direction = _Direction; }

	virtual void Render(CRenderManager *RenderManager);
	virtual const Mat44f & GetTransform();
	void SetShadowMap(CRenderManager &RenderManager);
};

#endif //DIRECTIONAL_LIGHT_H