#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight(const std::string &_LevelId);
	COmniLight(std::string _name, const std::string &_LevelId);
	COmniLight(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
	bool const GetInsideFrustum();
	void SetShadowMap(CRenderManager &RenderManager);
	void Save(FILE* _File);
};

#endif //OMNI_LIGHT_H