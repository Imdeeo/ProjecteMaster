#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utils\Named.h"
#include <vector>

class CTexture;
class CMaterialParameter;
class CRenderableObjectTechnique;

#define MAX_TEXTURES 10

class CMaterial : public CNamed
{
private:
	CTexture* m_Textures[MAX_TEXTURES];
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	void Destroy();
public:
	CMaterial(const CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void Apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	CRenderableObjectTechnique* GetRenderableObjectTechnique();
	void * GetNextParameterAddress(unsigned int NumBytes);
	const std::vector<CMaterialParameter *> & GetParameters(){ return m_Parameters;}
	void CMaterial::operator=(CMaterial &b);
	CTexture * GetTexture(int index = 0);
	void SetValue(int _index, float _value);
	void Save(FILE* _File);
};

#endif //MATERIAL_H