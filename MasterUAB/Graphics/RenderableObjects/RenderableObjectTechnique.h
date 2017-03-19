#ifndef H_RENDERABLEOBJECTTECHNIQUE_H
#define H_RENDERABLEOBJECTTECHNIQUE_H

#include "Utils\Named.h"

class CEffectTechnique;

class CRenderableObjectTechnique : public CNamed
{
private:
	CEffectTechnique *m_EffectTechnique;
public:
	CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);
	void SetEffectTechnique(CEffectTechnique *EffectTechnique);
	CEffectTechnique * GetEffectTechnique() const;
};

#endif // H_RENDERABLEOBJECTTECHNIQUE_H