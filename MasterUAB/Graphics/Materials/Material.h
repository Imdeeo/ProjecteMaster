#ifndef MATERIAL_H
#define MATERIAL_H

#include "Named.h"
#include <vector>
#include "Utils.h"

#include <assert.h>

#include "Texture\Texture.h"
#include "Effects\EffectTechnique.h"

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_Textures;
	CEffectTechnique *m_EffectTechnique;
	void Destroy();
public:
	CMaterial(const CXMLTreeNode &TreeNode);
	virtual ~CMaterial(){}
	virtual void Apply();
	UAB_GET_PROPERTY(CEffectTechnique*, EffectTechnique);
};

#endif //MATERIAL_H