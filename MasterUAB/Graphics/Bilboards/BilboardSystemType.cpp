#include "BilboardSystemType.h"
#include "Engine\UABEngine.h"
#include "Materials\MaterialManager.h"
#include "LevelManager\LevelManager.h"

CBilboardSystemType::CBilboardSystemType(tinyxml2::XMLElement* TreeNode, CLevel * _Level) : CNamed(TreeNode), CLevelInfo(_Level)
{
	m_Level = _Level->GetName();
	m_Material = _Level->GetMaterialManager()->GetResource(TreeNode->GetPszProperty("material"));
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