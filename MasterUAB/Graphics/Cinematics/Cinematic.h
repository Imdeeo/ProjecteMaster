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
	CCinematic(CXMLTreeNode &TreeNode);
	virtual ~CCinematic();
	virtual void Stop();
	virtual void Play(bool Cycle);
	virtual void Pause();
	void LoadXML(const std::string &Filename);
	void AddCinematicObject(CCinematicObject *CinematicObject);
	void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
};

#endif H_CINEMATIC_H