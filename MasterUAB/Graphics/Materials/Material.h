#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utils.h"
#include "Utils\Named.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include <vector>

class CTexture;
class CMaterialParameter;
class CRenderableObjectTechnique;

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_Textures;
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	unsigned int m_ReflectionStageId;
	void Destroy();
public:
	CMaterial(const CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void Apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	UAB_GET_PROPERTY(CRenderableObjectTechnique*, RenderableObjectTechnique);
	void * GetNextParameterAddress(unsigned int NumBytes);
	const std::vector<CMaterialParameter *> & GetParameters(){ return m_Parameters;}
	void CMaterial::operator=(CMaterial &b);
};

#endif //MATERIAL_H