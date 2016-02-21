#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:
	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);
public:
	CSpotLight();
	CSpotLight(CXMLTreeNode &TreeNode);
	UAB_BUILD_GET_SET(float, Angle);
	UAB_BUILD_GET_SET(float, FallOff);

	void Render(CRenderManager *_RenderManager);

	virtual const Mat44f & GetTransform();

	void SetShadowMap(CRenderManager &RenderManager);
};

#endif //SPOT_LIGHT_H