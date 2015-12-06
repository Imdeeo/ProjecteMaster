#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight();
	CDirectionalLight(CXMLTreeNode &TreeNode);
	UAB_BUILD_GET_SET_BY_REFERENCE(Vect3f, Direction);
	virtual void Render(CRenderManager *RenderManager);
};

#endif //DIRECTIONAL_LIGHT_H