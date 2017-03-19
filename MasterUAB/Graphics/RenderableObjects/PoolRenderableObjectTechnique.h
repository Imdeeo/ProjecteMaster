#ifndef H_POOLRENDERABLEOBJECTTECHNIQUE_H
#define H_POOLRENDERABLEOBJECTTECHNIQUE_H

#include "Utils\Named.h"
#include "RenderableObjectTechnique.h"
#include "XML\tinyxml2.h"
#include <vector>

class CPoolRenderableObjectTechnique : public CNamed
{
private:
	class CPoolRenderableObjectTechniqueElement
	{
	public:
		CRenderableObjectTechnique m_RenderableObjectTechnique;
		CRenderableObjectTechnique *m_OnRenderableObjectTechniqueManager;
		CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager):
			m_RenderableObjectTechnique(Name,EffectTechnique), m_OnRenderableObjectTechniqueManager(OnRenderableObjectTechniqueManager){}
	};
	std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;
public:
	CPoolRenderableObjectTechnique(tinyxml2::XMLElement* TreeNode);
	
	virtual ~CPoolRenderableObjectTechnique();
	void Destroy();
	void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
	void Apply();

	CPoolRenderableObjectTechnique & CPoolRenderableObjectTechnique::operator=(CPoolRenderableObjectTechnique _PoolRenderableObjectTechnique);
};

#endif // H_POOLRENDERABLEOBJECTTECHNIQUE_H