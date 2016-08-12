#ifndef H_GAME_PLAY_MANAGER_H
#define H_GAME_PLAY_MANAGER_H

#include <stdlib.h>
#include <vector>

class CLUAComponent;

class CGamePlayManager
{
private:
	std::vector<CLUAComponent *> m_Resources;
public:
	CGamePlayManager();
	~CGamePlayManager();
	void Update(float ElapsedTime);
	void Destroy();
	void Clear();
	void AddComponent(CLUAComponent* _LUAComponent);
};

#endif //H_GAME_PLAY_MANAGER_H