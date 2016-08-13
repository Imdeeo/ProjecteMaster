#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "Utils\Named.h"

class CEffectVertexShader;
class CEffectPixelShader;
class CEffectGeometryShader;

class CEffectTechnique: public CNamed
{
private:
	CEffectVertexShader *m_VertexShader;
	CEffectPixelShader *m_PixelShader;
	CEffectGeometryShader *m_GeometryShader;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
	std::string m_GeometryShaderName;
public:
	CEffectTechnique(CXMLTreeNode &TreeNode);
	CEffectTechnique(CEffectVertexShader* _EffectVertexShader, CEffectPixelShader* _EffectPixelShader, CEffectGeometryShader* _EffectGeometryShader, const std::string _Name);
	virtual ~CEffectTechnique();
	
	CEffectVertexShader* GetVertexShader();
	CEffectPixelShader* GetPixelShader();
	CEffectGeometryShader* GetGeometryShader();
	void Refresh();
	void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer);
};

#endif //EFFECT_TECHNIQUE_H