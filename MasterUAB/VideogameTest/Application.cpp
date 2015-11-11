#include "Application.h"

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "ContextManager.h"
#include "DebugRender.h"

CApplication::CApplication(CDebugRender *_DebugRender, CContextManager *_ContextManager)
	: m_DebugRender(_DebugRender)
	, m_ContextManager(_ContextManager)
	, m_WorldRotation(0)
{
}


CApplication::~CApplication()
{
}


void CApplication::Update(float _ElapsedTime)
{
	// TODO

	// DWORD l_CurrentTime = timeGetTime();
	// m_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
	// m_PreviousTime = l_CurrentTime;
	
	m_WorldRotation += 1.13f * _ElapsedTime;
	while (m_WorldRotation > FLOAT_PI_VALUE * 2)
	{
		m_WorldRotation -= FLOAT_PI_VALUE * 2;
	}
}

void CApplication::Render()
{
	m_ContextManager->BeginRender();

	CCamera camera;
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(8.0f / 6.0f);
	camera.SetZNear(0.1f);
	camera.SetZFar(50.f);

	camera.SetPosition(Vect3f(5, 10, 20));
	camera.SetLookAt(Vect3f(0, 0, 0));
	camera.SetUp(Vect3f(0, 1, 0));

	camera.SetMatrixs();

	Mat44f world;

	world.SetIdentity();
	world.RotByAnglesYXZ(m_WorldRotation, 0, 0);

	/*
	Mat44f world, view, projection;
	view.SetIdentity();
	view.SetFromLookAt(Vect3f(5, 10, 20), Vect3f(0, 0, 0), Vect3f(0, 1, 0));

	projection.SetIdentity();
	projection.SetFromPerspective(1.047f, 8.0f / 6.0f, 0.1f, 50.0f);
	*/

	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->SetCamera(camera);

	m_ContextManager->SetDebugSize(5);
	m_ContextManager->SetBaseColor(CColor(1, 1, 1, 1));

	m_ContextManager->Draw(m_DebugRender->GetSimpleCube(), CContextManager::RS_BACKFACES);


	m_ContextManager->EndRender();
}

