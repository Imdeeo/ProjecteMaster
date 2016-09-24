#include "LevelInfo.h"

#include <string.h>

CLevelInfo::CLevelInfo()
{
}


CLevelInfo::~CLevelInfo()
{
}

bool CLevelInfo::IsInLevel(std::string _LevelId)
{
	for (size_t i = 0; i < m_Levels.size(); i++)
	{
		//if (strcmpi(m_Levels[i], _LevelId))
		if (!m_Levels[i].compare(_LevelId))
		{
			return true;
		}
	}
	return false;
}
void CLevelInfo::AddLevel(std::string _LevelId)
{
	m_Levels.push_back(_LevelId);
}
void CLevelInfo::RemoveLevel(std::string _LevelId)
{
	for (size_t i = 0; i < m_Levels.size(); i++)
	{
		//if (strcmpi(m_Levels[i], _LevelId))
		if (!m_Levels[i].compare(_LevelId))
		{
			m_Levels.erase(m_Levels.begin() + i);
		}
	}
}
bool CLevelInfo::HasAnyLevel()
{
	return m_Levels.size() > 0;
}
