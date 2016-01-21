#include "PoolRenderableObjectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Engine\UABEngine.h"

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{

}

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