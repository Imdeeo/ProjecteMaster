#ifndef H_CINEMATIC_H
#define H_CINEMATIC_H

#include <stdlib.h>
#include <vector>
#include "RenderableObjects\RenderableObject.h"
#include "CinematicPlayer.h"
#include "Utils.h"

class CCinematicObject;

class CCinematic : public CCinematicPlayer, public CNamed
{
protected:
	std::vector<CCinematicObject *> m_CinematicObjects;
public:
	CCinematic(CXMLTreeNode _Input);
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