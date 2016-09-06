#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:

#ifdef _DEBUG
	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);
#endif

	float m_Angle;
	float m_FallOff;
public:
	CSpotLight();
	CSpotLight(std::string _name);
	CSpotLight(CXMLTreeNode &TreeNode);
	bool const GetInsideFrustum();
	float GetAngle()const{ return m_Angle; }
	float GetFallOff()const{ return m_FallOff; }
	void SetAngle(float _Angle){ m_Angle = _Angle; }
	void SetFallOff(float _FallOff){ m_FallOff = _FallOff; }

	CEmptyPointerClass* GetAngleLuaAdress()
	{
		return (CEmptyPointerClass*)&m_Angle;
	}

	CEmptyPointerClass* GetFallOffLuaAdress()
	{
		return (CEmptyPointerClass*)&m_FallOff;
	}

#ifdef _DEBUG
	void Render(CRenderManager *_RenderManager);
#endif

	virtual const Mat44f & GetTransform();

	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
};

#endif //SPOT_LIGHT_H