#include "BilboardSystemType.h"
#include "Engine\UABEngine.h"
#include "Materials\MaterialManager.h"
#include "LevelManager\LevelManager.h"

CBilboardSystemType::CBilboardSystemType(tinyxml2::XMLElement* TreeNode, const std::string &_LevelId) : CNamed(TreeNode), CLevelInfo(_LevelId)
{
	m_Material = UABEngine.GetLevelManager()->GetResource(m_Level)->GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
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