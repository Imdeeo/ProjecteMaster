#ifndef H_CINEMATIC_H
#define H_CINEMATIC_H

#include <stdlib.h>
#include <vector>
#include "RenderableObjects\RenderableObject.h"
#include "CinematicPlayer.h"
#include "Utils.h"
#include "XML\tinyxml2.h"
#include "Utils\LevelInfo.h"

class CCinematicObject;
class CLevel;

class CCinematic : public CCinematicPlayer, public CNamed, public CLevelInfo
{
protected:
	std::vector<CCinematicObject *> m_CinematicObjects;
public:
	CCinematic(tinyxml2::XMLElement* TreeNode,CLevel* _Level);
	virtual ~CCinematic();
	void AddCinematicObject(CCinematicObject *CinematicObject);
	void Update(float _ElapsedTime);
	void Play();
	void Stop();
	void Pause();
	void OnRestartCycle();
	bool IsFinished();
};

#endif H_CINEMATIC_H