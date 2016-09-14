#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utils\Named.h"
#include <vector>
#include <string.h>
#include "XML\tinyxml2.h"

class CTexture;
class CMaterialParameter;
class CRenderableObjectTechnique;

#define MAX_TEXTURES 11

class CMaterial : public CNamed
{
private:
	CTexture* m_Textures[MAX_TEXTURES];
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	void Destroy();
public:
	CMaterial(tinyxml2::XMLElement* TreeNode);
	virtual ~CMaterial();
	virtual void Apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);
	CRenderableObjectTechnique* GetRenderableObjectTechnique();
	void * GetNextParameterAddress(unsigned int NumBytes);
	const std::vector<CMaterialParameter *> & GetParameters(){ return m_Parameters;}
	void CMaterial::operator=(CMaterial &b);
	CTexture * GetTexture(int index = 0);
	void SetValue(int _index, float _value);
	float GetValue(int _index);
	void Save(FILE* _File);
};

#endif //MATERIAL_H