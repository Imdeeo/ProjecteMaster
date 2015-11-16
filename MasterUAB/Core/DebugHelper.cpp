#include "DebugHelper.h"

static CDebugHelper *s_CurrentDebugHelper;

class CDummyDebugHelper : public CDebugHelper
{
public:
	CDummyDebugHelper()
	{
		s_CurrentDebugHelper = this;
	}
};

static CDummyDebugHelper s_DummyDebugHelper;



CDebugHelper* CDebugHelper::GetDebugHelper()
{
	return s_CurrentDebugHelper;
}

void CDebugHelper::SetCurrentDebugHelper(CDebugHelper* _DebugHelper)
{
	if (_DebugHelper)
	{
		s_CurrentDebugHelper = _DebugHelper;
	}
	else
	{
		s_CurrentDebugHelper = &s_DummyDebugHelper;
	}
}
