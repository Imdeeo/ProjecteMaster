#ifndef LEVEL_INO_H
#define LEVEL_INO_H

#include <string>
#include <vector>

class CLevelInfo
{
public:
	CLevelInfo(const std::string &_LevelId);
	virtual ~CLevelInfo();

	std::string m_Level;
};

#endif