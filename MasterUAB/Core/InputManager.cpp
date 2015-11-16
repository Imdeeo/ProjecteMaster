#include "InputManager.h"

static CInputManager* s_CurrentInputManager;

class CDummyInputManager : public CInputManager
{
public:
	CDummyInputManager()
	{
		s_CurrentInputManager = this;
	}
};

static CDummyInputManager s_DummyInputManager;


CInputManager* CInputManager::GetInputManager()
{
	return s_CurrentInputManager;
}

void CInputManager::SetCurrentInputManager(CInputManager* _InputManager)
{
	if (_InputManager)
	{
		s_CurrentInputManager = _InputManager;
	}
	else
	{
		s_CurrentInputManager = &s_DummyInputManager;
	}
}
