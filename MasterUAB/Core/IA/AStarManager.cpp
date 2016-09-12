#include "AStarManager.h"
#include "Engine\UABEngine.h"

CAStarManager::CAStarManager(void)
{
}

CAStarManager::~CAStarManager(void)
{
	Destroy();
}

bool CAStarManager::Load(const std::string &FileName)
{
	m_Filename = FileName;

	return AddResource("level_" + UABEngine.GetLevelLoaded(), new CAStar(m_Filename));
}

bool CAStarManager::Reload()
{
	Destroy();
	return Load(m_Filename);
}