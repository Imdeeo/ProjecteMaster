#ifndef LOADSCREEN_MANAGER_H
#define LOADSCREEN_MANAGER_H

#include <string>
#include <vector>

class CEffectTechnique;
class CRenderManager;
class CTexture;

class CLoadScreenManager
{
private:
	std::string m_Filename;

	CEffectTechnique* m_LoadTechnique;

	struct TLoadScreen {
		std::string name;
		float time;
		std::string file;
		bool animated;
	};

	std::vector<TLoadScreen*> m_LoadScreens;
	int m_Count;
	float m_Timer;
	float m_Angle;
	bool m_Loading;
	void *m_EffectAddress;
	CTexture* m_Texture;
	void LoadScreensXML(std::string _filename);	
	void LoadScreen(const std::string _FileName);
public:
	CLoadScreenManager(const std::string &Filename);
	virtual ~CLoadScreenManager(void);
	void Load();
	void Reload();
	void Destroy();
	void RenderLoadScreen();
	void SetLoading(bool _Loading){ m_Loading = _Loading; };
};

#endif //LOADSCREEN_MANAGER_H