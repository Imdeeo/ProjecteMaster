#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
protected:


	virtual CRenderableVertexs* GetShape(CRenderManager *_RenderManager);


	float m_Angle;
	float m_FallOff;
public:
	CSpotLight(CLevel* _Level);
	CSpotLight(std::string _name, CLevel* _Level);
	CSpotLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
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

	void Render(CRenderManager *_RenderManager);

	virtual const Mat44f & GetTransform();

	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
};

#endif //SPOT_LIGHT_H