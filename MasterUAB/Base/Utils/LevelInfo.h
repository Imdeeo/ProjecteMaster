#ifndef LEVEL_INO_H
#define LEVEL_INO_H

#include <string>

class CLevel;

class CLevelInfo
{
public:
	CLevelInfo(CLevel* _Level);
	CLevelInfo(const std::string &_LevelId);
	virtual ~CLevelInfo();

	std::string m_Level;
};

#endif
