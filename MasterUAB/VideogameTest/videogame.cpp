#include <Windows.h>
#include <Windowsx.h>

#include "no_sillywarnings_please.h"

#include <d3d11.h>

#include <cassert>

#include <AntTweakBar.h>

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "Application.h"

#include "InputManager\InputManager.h"
#include "DebugHelper\DebugHelperImplementation.h"

#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\Effect.h"
#include "Camera\Camera.h"


#pragma comment(lib, "Winmm.lib")

#define APPLICATION_NAME	"VIDEOGAME"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

bool doExit = false;
bool hasFocus = true;

CContextManager s_Context;

void ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition)
{
	// This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLongW(Window, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
		if (GetWindowPlacement(Window, &WindowPosition) &&
			GetMonitorInfoW(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLongW(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(Window, HWND_TOP,
				MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
				MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
				MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLongW(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(Window, &WindowPosition);
		SetWindowPos(Window, 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			// TODO: Resetear el AntTeakBar
			TwWindowSize(0, 0);
			s_Context.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			// TODO: Resetear el AntTeakBar
			TwWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		doExit = true;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	//-----------Para detectar Memory Leaks-------------------------
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(32455);
	//----------

	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", "A Simple Song", NULL);
		return 1;
	}

#define OJUCUIDAO
#ifndef OJUCUIDAO//_DEBUG
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);

	// Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, desktop.left, desktop.top, desktop.right, desktop.bottom, NULL, NULL, wc.hInstance, NULL);

	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;
	RECT windowBoundary;

	EnumDisplaySettings(NULL, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = desktop.right;
	fullscreenSettings.dmPelsHeight = desktop.bottom;
	/*fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;*/
	fullscreenSettings.dmFields = DM_PELSWIDTH |
		DM_PELSHEIGHT |
		DM_BITSPERPEL |
		DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, desktop.right, desktop.bottom, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hWnd, SW_MAXIMIZE);

	s_Context.CreateContext(hWnd, desktop.right, desktop.bottom);

	s_Context.CreateBackBuffer(hWnd, desktop.right, desktop.bottom);
#else

	// Calcular el tamano de nuestra ventana
	RECT rc = {
		0, 0, 1280, 720
	};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the application's window
	HWND hWnd = CreateWindow(
		APPLICATION_NAME,
		APPLICATION_NAME,
		WS_OVERLAPPEDWINDOW,
		100, 100,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL,
		NULL,
		wc.hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "A Simple Song", NULL);
		return 1;
	}

	// Añadir aquí el Init de la applicacioón

	s_Context.CreateContext(hWnd, 1280, 720);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	s_Context.CreateBackBuffer(hWnd, 1280, 720);
#endif

	s_Context.InitStates();
	
	//CDebugRender debugRender(s_Context.GetDevice());

	CDebugHelperImplementation debugHelper(s_Context.GetDevice());
	CDebugHelper::SetCurrentDebugHelper(&debugHelper);

	CApplication application(&s_Context);

	application.Init();

	CInputManager* l_InputManager = UABEngine.GetInputManager();
	l_InputManager->SetWindow(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	UpdateWindow(hWnd);
	DWORD l_PreviousTime = timeGetTime();
	DWORD l_CurrentTime;
	float l_ElapsedTime;

	while (!doExit)
	{
		l_InputManager->Update();

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (!debugHelper.Update(msg.hwnd, msg.message, msg.wParam, msg.lParam))
				l_InputManager->GetManager()->HandleMessage(msg);

			switch (msg.message)
			{
				case WM_SETFOCUS:
					hasFocus = true;
					break;
				case WM_KILLFOCUS:
					hasFocus = false;
					break;
				case WM_MOUSEMOVE:
					if (hasFocus)
						l_InputManager->UpdateAxis(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
					break;
				case WM_KEYUP:
					bool Alt = ((msg.lParam & (1 << 29)) != 0);
					if (msg.wParam == VK_RETURN && Alt)
					{
						WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
						GetWindowPlacement(msg.hwnd, &windowPosition);

						ToggleFullscreen(msg.hwnd, windowPosition);
					}
					break;
			}
		}
		
		l_CurrentTime = timeGetTime();
		l_ElapsedTime = (float)(l_CurrentTime - l_PreviousTime)*0.001f;
		l_PreviousTime = l_CurrentTime;

		application.Update(l_ElapsedTime);
		application.Render();
	}

	UnregisterClass(APPLICATION_NAME, wc.hInstance);
	
	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	s_Context.Dispose();

	return 0;
}
