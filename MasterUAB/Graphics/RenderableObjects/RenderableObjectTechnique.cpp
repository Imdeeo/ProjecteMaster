#include "RenderableObjectTechnique.h"
#include "Effects\EffectTechnique.h"

CRenderableObjectTechnique::CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique) : CNamed(Name)
{
	m_EffectTechnique = EffectTechnique;
}

void CRenderableObjectTechnique::SetEffectTechnique(CEffectTechnique *EffectTechnique)
{
	m_EffectTechnique = EffectTechnique;
}

CEffectTechnique * CRenderableObjectTechnique::GetEffectTechnique() const
{
	return m_EffectTechnique;
}