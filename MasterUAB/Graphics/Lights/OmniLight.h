#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

class CLevel;

class COmniLight : public CLight
{
public:
	COmniLight(CLevel* _Level);
	COmniLight(std::string _name, CLevel* _Level);
	COmniLight(tinyxml2::XMLElement* TreeNode, CLevel* _Level);
	bool const GetInsideFrustum();
	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
	void Render(CRenderManager *RenderManager);
	CRenderableVertexs* GetShape(CRenderManager *_RenderManager);
	const Mat44f & GetTransform();
};

#endif //OMNI_LIGHT_H