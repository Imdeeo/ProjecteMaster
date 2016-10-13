#include "AStarManager.h"
#include "Engine\UABEngine.h"

CAStarManager::CAStarManager(void)
{
}

CAStarManager::~CAStarManager(void)
{
	Destroy();
}

bool CAStarManager::Load(const std::string &_FileName, const std::string &_LevelId)
{
	m_Filename = _FileName;
	m_LevelId = _LevelId;

	return AddResource(_LevelId, new CAStar(m_Filename));
}

bool CAStarManager::Reload()
{
	Destroy();
	return Load(m_Filename,m_LevelId);
}