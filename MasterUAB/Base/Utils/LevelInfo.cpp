#include "LevelInfo.h"

#include <string.h>

#include "LevelManager\Level.h"

CLevelInfo::CLevelInfo(CLevel* _Level)
{
	m_Level = _Level->GetName();
}

CLevelInfo::CLevelInfo(const std::string &_LevelId)
{
	m_Level = _LevelId;
}

CLevelInfo::~CLevelInfo()
{
}
