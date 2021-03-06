#ifndef LIGHT_H
#define LIGHT_H

#include "Utils\Named.h"
#include "3DElement\3DElement.h"
#include "XML\tinyxml2.h"
#include <vector>

#include "Utils\LevelInfo.h"

#include "Math\Color.h"

class CDynamicTexture;
class CTexture;
class CRenderableVertexs;
class CRenderManager;
class CRenderableObjectsManager;
class CFrustum;
class CLevel;

class CLight : public CNamed, public C3DElement, public CLevelInfo
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

	CFrustum *m_Frustum;

	void Destroy();

public:

	CLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	CLight(std::string _name, CLevel* _Level);
	CLight(CLevel* _Level);
	virtual ~CLight();
	//UAB_BUILD_GET_SET_BY_REFERENCE(Vect3f, Position);
	const CColor & GetColor() const{ return m_Color; }

	float GetIntensity()const { return m_Intensity; }
	float GetStartRangeAttenuation()const { return m_StartRangeAttenuation; }
	float GetEndRangeAttenuation()const { return m_EndRangeAttenuation; }
	bool  GetEnabled()const { return m_Enabled; }
	TLightType GetType()const { return m_Type; }

	CEmptyPointerClass* GetPositionLuaAddress(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Position[index];
	}

	CEmptyPointerClass* GetEnableLuaAdress()
	{
		return (CEmptyPointerClass*)&m_Enabled;
	}

	CEmptyPointerClass* GetIntensityLuaAdress()
	{
		return (CEmptyPointerClass*)&m_Intensity;
	}

	CEmptyPointerClass* GetStartRangeAttenuationLuaAdress()
	{
		return (CEmptyPointerClass*)&m_StartRangeAttenuation;
	}

	CEmptyPointerClass* GetEndRangeAttenuattionLuaAdress()
	{
		return (CEmptyPointerClass*)&m_EndRangeAttenuation;
	}

	CEmptyPointerClass* GetColorLuaAdress()
	{
		return (CEmptyPointerClass*)&m_Color;
	}

	CEmptyPointerClass* GetGenerateShadowMapLuaAdress()
	{
		return (CEmptyPointerClass*)&m_GenerateShadowMap;
	}

	void SetColor(CColor _Color) { m_Color = _Color; }

	void SetIntensity(float _Intensity) { m_Intensity = _Intensity; }
	void SetStartRangeAttenuation(float _StartRangeAttenuation) { m_StartRangeAttenuation = _StartRangeAttenuation; }
	void SetEndRangeAttenuation(float _EndRangeAttenuation) { m_EndRangeAttenuation = _EndRangeAttenuation; }
	void SetEnabled(bool _Enabled) { m_Enabled = _Enabled; }
	void SetType(TLightType _Type) { m_Type = _Type; }

#ifdef _DEBUG
	virtual void Render(CRenderManager *RenderManager);
#endif
	static TLightType GetLightTypeByName(const std::string &StrLightType);

	virtual const Mat44f & GetTransform();

	virtual const bool GetInsideFrustum();

	// Shadowmap
	void SetGenerateShadowMap(bool _GenerateShadowMap){ m_GenerateShadowMap = _GenerateShadowMap; }
	bool GetGenerateShadowMap()const{ return m_GenerateShadowMap; }
	void CreateShadowMap(CDynamicTexture *_ShadowMap){ m_ShadowMap = _ShadowMap; m_ShadowMaskTexture = nullptr; };
	CDynamicTexture* GetShadowMap();
	CTexture* GetShadowMaskTexture();
	const Mat44f & GetViewShadowMap()const{ return m_ViewShadowMap; }
	const Mat44f & GetProjectionShadowMap()const{ return m_ProjectionShadowMap; }
	std::vector<CRenderableObjectsManager *> & GetLayers() {return m_Layers;}
	virtual void SetShadowMap(CRenderManager &RenderManager) = 0;
	virtual void Save(FILE* _File) = 0;
};

#endif //LIGHT_H