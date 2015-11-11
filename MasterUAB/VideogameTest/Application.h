#pragma once

class CDebugRender;
class CContextManager;


class CApplication
{
public:
	CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager);
	~CApplication();

	void Update(float _ElapsedTime);
	void Render();

private:

	CDebugRender *m_DebugRender;
	CContextManager *m_ContextManager;


	float m_WorldRotation;
};

