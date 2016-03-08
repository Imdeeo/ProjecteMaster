#include "PoolRenderableObjectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"
#include "Effects\EffectManager.h"

/*CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	for (size_t i = 0; i < TreeNode.GetNumChildren(); i++)
	{
		CXMLTreeNode l_Element = TreeNode(i);
		if (l_Element.GetName() == "default_technique")
		{
			//TODO
		}
		if (l_Element.GetName() == "renderable_object_technique")
		{
			std::string l_name = l_Element.GetPszProperty("name");
			std::string l_technique = l_Element.GetPszProperty("technique");
			UABEngine.GetRenderableObjectTechniqueManager()->AddResource(l_name, new CRenderableObjectTechnique(l_name, nullptr));
			m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(l_name, UABEngine.GetEffectManager()->GetResource(l_technique), UABEngine.GetRenderableObjectTechniqueManager()->GetResource(l_name)));
		}
	}
}*/
CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode) :CNamed(TreeNode)
{}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
	Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
	for(int i = 0; i<m_RenderableObjectTechniqueElements.size(); i++)
	{
		delete m_RenderableObjectTechniqueElements[i];
	}
	m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
	m_RenderableObjectTechniqueElements.push_back(new CPoolRenderableObjectTechniqueElement(Name, UABEngine.GetEffectManager()->GetResource(TechniqueName), ROTOnRenderableObjectTechniqueManager));
}
	
void CPoolRenderableObjectTechnique::Apply()
{
	for(int i = 0; i<m_RenderableObjectTechniqueElements.size(); i++)
	{
		m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique());
	}
}

CPoolRenderableObjectTechnique & CPoolRenderableObjectTechnique::operator=(CPoolRenderableObjectTechnique _PoolRenderableObjectTechnique)
{
	return *this;
}