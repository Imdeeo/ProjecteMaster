#ifndef H_CINEMATIC_MANAGER_H
#define H_CINEMATIC_MANAGER_H

#include <stdlib.h>
#include <vector>
#include "RenderableObjects\RenderableObject.h"
#include "Utils\TemplatedMapManager.h"
#include "Cinematic.h"

class CLevel;

class CCinematicManager : public CRenderableObject, public CTemplatedLevelMapManager<CCinematic>
{
protected:
	std::vector<CCinematicObject *> m_CinematicObjects;
	std::string m_FileName;
	std::string m_LevelName;
public:
	CCinematicManager(CLevel*);
	virtual ~CCinematicManager();
	void LoadXML(const std::string &FileName, CLevel* _Level);
	void Reload();
	void Update(float _ElapsedTime);
	virtual void Render(CRenderManager *RenderManager){};
};

#endif //H_CINEMATIC_MANAGER_H