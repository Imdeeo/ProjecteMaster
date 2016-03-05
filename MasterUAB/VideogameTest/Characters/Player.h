#ifndef C_PLAYER_H
#define C_PLAYER_H

#include "Utils.h"
#include "Character.h"

class CCameraController;

class CPlayer :	public CCharacter
{
protected:
	std::string			m_CameraControllerName;
	CCameraController * m_CameraController;
public:
	CPlayer(const CXMLTreeNode&);
	virtual ~CPlayer();

	CCameraController* GetCameraController();
	void SetCameraController(CCameraController*);

	virtual void Update(float _ElapsedTime){}

	virtual bool Refresh();
};

#endif