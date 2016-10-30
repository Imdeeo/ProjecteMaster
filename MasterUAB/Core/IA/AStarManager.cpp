#include "AStarManager.h"
#include "Engine\UABEngine.h"

#include "LevelManager\LevelManager.h"

CAStarManager::CAStarManager(void)
{
}

CAStarManager::~CAStarManager(void)
{
	Destroy();
}

bool CAStarManager::Load(const std::string &_FileName, CLevel* _Level)
{
	m_Filename = _FileName;
	m_LevelId = _Level->GetName();

	return AddResource(m_LevelId, new CAStar(m_Filename));
}

bool CAStarManager::Reload()
{
	Destroy();
	return Load(m_Filename, UABEngine.GetLevelManager()->GetResource(m_LevelId));
}