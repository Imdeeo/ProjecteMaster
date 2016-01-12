#ifndef H_CINEMATIC_H
#define H_CINEMATIC_H

#include <stdlib.h>
#include <vector>
#include "RenderableObjects\RenderableObject.h"
#include "CinematicPlayer.h"

class CCinematicObject;

class CCinematic : public CRenderableObject, public CCinematicPlayer
{
protected:
	std::vector<CCinematicObject *> m_CinematicObjects;
public:
	CCinematic();
	virtual ~CCinematic();
	void LoadXML(const std::string &FileName);
	void AddCinematicObject(CCinematicObject *CinematicObject);
	void Update(float _ElapsedTime);
	virtual void Render(CRenderManager *RenderManager){};

};

#endif H_CINEMATIC_H