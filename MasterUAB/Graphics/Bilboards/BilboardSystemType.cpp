#include "BilboardSystemType.h"
#include "Engine\UABEngine.h"
#include "Materials\MaterialManager.h"

CBilboardSystemType::CBilboardSystemType(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CNamed(TreeNode), m_LevelId(_LevelId)
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