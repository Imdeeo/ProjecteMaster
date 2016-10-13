#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight();
	COmniLight(std::string _name);
	COmniLight(tinyxml2::XMLElement* TreeNode);
	bool const GetInsideFrustum();
	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
	void Render(CRenderManager *RenderManager);
	CRenderableVertexs* GetShape(CRenderManager *_RenderManager);
	const Mat44f & GetTransform();
};

#endif //OMNI_LIGHT_H