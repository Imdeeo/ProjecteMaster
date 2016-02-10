#ifndef LIGHT_H
#define LIGHT_H

#include "Utils\Named.h"
#include "Utils.h"
#include "Math\Color.h"
#include "3DElement\3DElement.h"
#include "Texture\DynamicTexture.h"

class CRenderManager;
class CRenderableObjectsManager;
class CEffectManager;

class CLight : public CNamed, public C3DElement
{
protected:
	CDynamicTexture *m_ShadowMap;
	CTexture *m_ShadowMaskTexture;
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

	// Shadowmap
	UAB_BUILD_GET_SET_AND_GET_ADDRESS(bool, GenerateShadowMap);
	UAB_GET_PROPERTY_POINTER(CDynamicTexture, ShadowMap);
	UAB_GET_PROPERTY_POINTER(CTexture, ShadowMaskTexture);
	UAB_BUILD_GET_BY_REFERENCE(Mat44f, ViewShadowMap);
	UAB_BUILD_GET_BY_REFERENCE(Mat44f, ProjectionShadowMap);
	UAB_BUILD_GET_BY_REFERENCE(std::vector<CRenderableObjectsManager *>, Layers);
	virtual void SetShadowMap(CRenderManager &RenderManager) = 0;
};

#endif //LIGHT_H