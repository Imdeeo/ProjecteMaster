#ifndef ANIMATEDMODELSMANAGER_H
#define ANIMATEDMODELSMANAGER_H

#include <string>

class CAnimatedModelsManager
{
private:
	std::string m_Filename;
public:
	CAnimatedModelsManager(void);
	virtual ~CAnimatedModelsManager(void);
	bool Load(const std::string &FileName);
	bool Reload();
};

#endif //ANIMATEDMODELSMANAGER_H
