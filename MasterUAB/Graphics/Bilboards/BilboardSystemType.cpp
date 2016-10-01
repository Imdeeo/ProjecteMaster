#include "BilboardSystemType.h"
#include "Engine\UABEngine.h"
#include "Materials\MaterialManager.h"

CBilboardSystemType::CBilboardSystemType(tinyxml2::XMLElement* TreeNode) : CNamed(TreeNode)
{
	m_Material = UABEngine.GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
	m_NumFrames = TreeNode->GetIntProperty("frames", 1);
	m_TimePerFrame = TreeNode->GetFloatProperty("time", 1.0f);
}

CBilboardSystemType::~CBilboardSystemType(void)
{
	Destroy();
}

void CBilboardSystemType::Destroy()
{
}