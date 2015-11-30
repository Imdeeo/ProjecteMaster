#ifndef MATERIAL_H
#define MATERIAL_H

#include "Named.h"
#include <vector>
#include "Utils.h"

#include <assert.h>

#include "Texture.h"
#include "EffectTechnique.h"

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_Textures;
	CEffectTechnique *m_EffectTechnique;
	void Destroy();
public:
	CMaterial(const CXMLTreeNode &TreeNode);
	virtual ~CMaterial(){}
	virtual void Apply()
	{assert(!"Can't call this method");}
	UAB_GET_PROPERTY(CEffectTechnique*, EffectTechnique);
};

#endif //MATERIAL_H