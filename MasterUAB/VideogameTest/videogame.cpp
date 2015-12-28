#include <Windows.h>
#include <Windowsx.h>

#include <d3d11.h>

#include <cassert>

// TODO: Activar AntTeakBar
#include <AntTweakBar.h>

#include "Math\Matrix44.h"
#include "Math\Vector4.h"

#include "Application.h"

#include "InputManager\InputManagerImplementation.h"
#include "DebugHelperImplementation.h"

#include "ContextManager\ContextManager.h"
#include "DebugRender.h"
#include "Effects\Effect.h"
#include "Camera\Camera.h"

#pragma comment(lib, "Graphics_d.lib")
#pragma comment(lib, "Winmm.lib")

#define APPLICATION_NAME	"VIDEOGAME"

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
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}//end switch( msg )

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	//-----------Para detectar Memory Leaks-------------------------
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(28668);
	//----------

	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx(&wc);


	// Calcular el tamano de nuestra ventana
	RECT rc = {
		0, 0, 800, 600
	};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	// Añadir aquí el Init de la applicacioón

	s_Context.CreateContext(hWnd, 800, 600);


	ShowWindow(hWnd, SW_SHOWDEFAULT);

	s_Context.CreateBackBuffer(hWnd, 800, 600);
	s_Context.InitStates();
	{
		//CDebugRender debugRender(s_Context.GetDevice());

		CInputManagerImplementation inputManager(hWnd);
		CInputManager::SetCurrentInputManager(&inputManager);

		inputManager.LoadCommandsFromFile("Data\\input.xml");

		CDebugHelperImplementation debugHelper(s_Context.GetDevice());
		CDebugHelper::SetCurrentDebugHelper(&debugHelper);

		CApplication application(&s_Context);

		application.Init();

		UpdateWindow(hWnd);
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		// Añadir en el while la condición de salida del programa de la aplicación
		DWORD m_PreviousTime = timeGetTime();

		bool hasFocus = true;

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				if (!debugHelper.Update(msg.hwnd, msg.message, msg.wParam, msg.lParam))
				{
					bool WasDown = false, IsDown = false, Alt = false;

					switch (msg.message)
					{
					case WM_SETFOCUS:
						hasFocus = true;
						inputManager.SetFocus(true);
						break;
					case  WM_KILLFOCUS:
						hasFocus = false;
						inputManager.SetFocus(false);
						break;
					case WM_SYSKEYDOWN:
					case WM_SYSKEYUP:
					case WM_KEYDOWN:
					case WM_KEYUP:
						WasDown = ((msg.lParam & (1 << 30)) != 0);
						IsDown = ((msg.lParam & (1 << 31)) == 0);
						Alt = ((msg.lParam & (1 << 29)) != 0);

						if (WasDown != IsDown)
						{
							if (IsDown)
							{
								bool consumed = false;
								switch (msg.wParam)
								{
								case VK_RETURN:
									if (Alt)
									{
										WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
										GetWindowPlacement(msg.hwnd, &windowPosition);

										ToggleFullscreen(msg.hwnd, windowPosition);
										consumed = true;
									}
									break;
								case VK_ESCAPE:
									PostQuitMessage(0);
									consumed = true;
									break;
								case VK_F4:
									if (Alt)
									{
										PostQuitMessage(0);
										consumed = true;
									}
									break;
								}
								if (consumed)
								{
									break;
								}
							}
						}
						if (!hasFocus || !inputManager.KeyEventReceived(msg.wParam, msg.lParam))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						break;
					case WM_MOUSEMOVE:
						if (hasFocus)
						{
							int xPosAbsolute = GET_X_LPARAM(msg.lParam);
							int yPosAbsolute = GET_Y_LPARAM(msg.lParam);

							inputManager.UpdateCursor(xPosAbsolute, yPosAbsolute);
						}
						else
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						break;
					default:
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			else
			{
				inputManager.BeginFrame();

				DWORD l_CurrentTime = timeGetTime();
				float m_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
				m_PreviousTime = l_CurrentTime;

				application.Update(m_ElapsedTime);
				application.Render();

				inputManager.EndFrame();
			}
		}
		UnregisterClass(APPLICATION_NAME, wc.hInstance);
	}
	// Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
	s_Context.Dispose();

	return 0;
}


