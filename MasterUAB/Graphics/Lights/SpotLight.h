#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "DirectionalLight.h"

class CSpotLight : public CLight
{
public:
	CSpotLight();
	CSpotLight(CXMLTreeNode &TreeNode);
	UAB_BUILD_GET_SET(float, Angle);
	UAB_BUILD_GET_SET(float, FallOff);
};

#endif //SPOT_LIGHT_H