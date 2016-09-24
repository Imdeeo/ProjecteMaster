#ifndef LEVEL_INO_H
#define LEVEL_INO_H

#include <string>
#include <vector>

class CLevelInfo
{
public:
	CLevelInfo();
	virtual ~CLevelInfo();

	bool IsInLevel(std::string _LevelId);
	void AddLevel(std::string _LevelId);
	void RemoveLevel(std::string _LevelId);
	bool HasAnyLevel();

private:
	std::vector<std::string> m_Levels;
};

#endif