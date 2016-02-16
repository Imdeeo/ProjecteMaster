#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight(CXMLTreeNode &TreeNode);
	void SetShadowMap(CRenderManager &RenderManager);
};

#endif //OMNI_LIGHT_H