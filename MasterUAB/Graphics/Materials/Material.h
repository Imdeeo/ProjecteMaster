#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utils\Named.h"
#include "Utils\LevelInfo.h"
#include <vector>
#include <string.h>
#include "XML\tinyxml2.h"

class CTexture;
class CMaterialParameter;
class CRenderableObjectTechnique;
class CLevel;

#define MAX_TEXTURES 11

class CMaterial : public CNamed,public CLevelInfo
{
private:
	CTexture* m_Textures[MAX_TEXTURES];
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	void Destroy();
public:
	CMaterial(tinyxml2::XMLElement* TreeNode, CLevel *_Level);
	CMaterial(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId);
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
	void CopyParameters(std::vector<CMaterialParameter *>_NewParameters, bool _destroy = true);
};

#endif //MATERIAL_H