#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CDirectionalLight : public CLight
{
protected:
	Vect2f m_OrthoShadowMapSize;
	Vect3f m_Direction;
public:
	CDirectionalLight(CLevel* _Level);
	CDirectionalLight(std::string _name, CLevel* _Level);
	CDirectionalLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	bool const GetInsideFrustum();
	const Vect3f & GetDirection() const { return m_Direction; }
	void SetDirection(Vect3f _Direction) { m_Direction = _Direction; }
	const Mat44f & CDirectionalLight::GetTransform();

	CEmptyPointerClass* GetDirectionLuaAdress(int index = 0)
	{
		return (CEmptyPointerClass*)&m_Direction[index];
	}

	virtual void Render(CRenderManager *RenderManager);

	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
};

#endif //DIRECTIONAL_LIGHT_H