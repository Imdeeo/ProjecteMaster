#ifndef LIGHT_H
#define LIGHT_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Color.h"
#include "3DElement\3DElement.h"

class CRenderManager;

class CLight : public CNamed, public C3DElement
{
public:
	enum TLightType
	{
		LIGHT_TYPE_OMNI = 0,
		LIGHT_TYPE_DIRECTIONAL,
		LIGHT_TYPE_SPOT,

		LIGHT_TYPE_NULL = -1
	};

	CLight(CXMLTreeNode &TreeNode);
	CLight();
	virtual ~CLight();
	//UAB_BUILD_GET_SET_BY_REFERENCE(Vect3f, Position);
	UAB_BUILD_GET_SET_BY_REFERENCE(CColor, Color);
	UAB_BUILD_GET_SET(float, Intensity);
	UAB_BUILD_GET_SET(float, StartRangeAttenuation);
	UAB_BUILD_GET_SET(float, EndRangeAttenuation);
	UAB_BUILD_GET_SET(bool, Enabled);
	UAB_BUILD_GET_SET(TLightType, Type);
	virtual void Render(CRenderManager *RenderManager);
	static TLightType GetLightTypeByName(const std::string &StrLightType);

	const Mat44f & GetTransform();
};

#endif //LIGHT_H