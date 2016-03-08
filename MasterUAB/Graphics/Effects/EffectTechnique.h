#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "Utils\Named.h"

class CEffectVertexShader;
class CEffectPixelShader;

class CEffectTechnique: public CNamed
{
private:
	CEffectVertexShader *m_VertexShader;
	CEffectPixelShader *m_PixelShader;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
public:
	CEffectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CEffectTechnique();
	
	CEffectVertexShader* GetVertexShader();
	CEffectPixelShader* GetPixelShader();
	void Refresh();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif //EFFECT_TECHNIQUE_H