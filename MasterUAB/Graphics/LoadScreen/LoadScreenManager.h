#ifndef LOADSCREEN_MANAGER_H
#define LOADSCREEN_MANAGER_H

#include <string>
#include <vector>

class CEffectTechnique;
class CRenderManager;

class CLoadScreenManager
{
private:
	std::string m_Filename;

	CEffectTechnique* m_LoadTechnique;

	struct TLoadScreen {
		std::string name;
		float time;
		std::string file;
	};

	std::vector<TLoadScreen*> m_LoadScreens;
	int m_Count;
	float m_Timer;

	void LoadScreensXML(std::string _filename);	
	void LoadScreen(const std::string _FileName);
public:
	CLoadScreenManager(void);
	CLoadScreenManager(const std::string &Filename);
	virtual ~CLoadScreenManager(void);
	void Load();
	void Reload();
	void Destroy();
	void RenderLoadScreen();
};

#endif //LOADSCREEN_MANAGER_H