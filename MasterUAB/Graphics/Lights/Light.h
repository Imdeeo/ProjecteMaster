#ifndef LIGHT_H
#define LIGHT_H

#include "Utils\Named.h"
#include "3DElement\3DElement.h"

#include <vector>

#include "Math\Color.h"

class CDynamicTexture;
class CTexture;
class CRenderableVertexs;
class CRenderManager;
class CRenderableObjectsManager;
class CXMLTreeNode;

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

protected:
	CDynamicTexture *m_ShadowMap;
	CTexture *m_ShadowMaskTexture;
	std::vector<CRenderableObjectsManager *> m_Layers;
	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);

	float m_Intensity;
	float m_StartRangeAttenuation;
	float m_EndRangeAttenuation;
	bool  m_Enabled;
	TLightType m_Type;

	bool m_GenerateShadowMap;

	Mat44f m_ViewShadowMap;
	Mat44f m_ProjectionShadowMap;

	CColor m_Color;

public:

	CLight(CXMLTreeNode &TreeNode);
	CLight();
	virtual ~CLight();
	//UAB_BUILD_GET_SET_BY_REFERENCE(Vect3f, Position);
	const CColor & GetColor() const{ return m_Color; }

	float GetIntensity()const { return m_Intensity; }
	float GetStartRangeAttenuation()const { return m_StartRangeAttenuation; }
	float GetEndRangeAttenuation()const { return m_EndRangeAttenuation; }
	bool  GetEnabled()const { return m_Enabled; }
	TLightType GetType()const { return m_Type; }

	void SetColor(CColor _Color) { m_Color = _Color; }

	void SetIntensity(float _Intensity) { m_Intensity = _Intensity; }
	void SetStartRangeAttenuation(float _StartRangeAttenuation) { m_StartRangeAttenuation = _StartRangeAttenuation; }
	void SetEndRangeAttenuation(float _EndRangeAttenuation) { m_EndRangeAttenuation = _EndRangeAttenuation; }
	void SetEnabled(bool _Enabled) { m_Enabled = _Enabled; }
	void SetType(TLightType _Type) { m_Type = _Type; }

	virtual void Render(CRenderManager *RenderManager);
	static TLightType GetLightTypeByName(const std::string &StrLightType);

	virtual const Mat44f & GetTransform();

	// Shadowmap
	void SetGenerateShadowMap(bool _GenerateShadowMap){ m_GenerateShadowMap = _GenerateShadowMap; }
	bool GetGenerateShadowMap()const{ return m_GenerateShadowMap; }
	bool GetGenerateShadowMapAddress()const{ return &m_GenerateShadowMap; }

	CDynamicTexture* GetShadowMap();
	CTexture* GetShadowMaskTexture();
	const Mat44f & GetViewShadowMap()const{ return m_ViewShadowMap; }
	const Mat44f & GetProjectionShadowMap()const{ return m_ProjectionShadowMap; }
	std::vector<CRenderableObjectsManager *> & GetLayers() {return m_Layers;}
	virtual void SetShadowMap(CRenderManager &RenderManager) = 0;
};

#endif //LIGHT_H