#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#define DIRECTINPUT_VERSION 0x0800

#include <string>
#include <gainput\gainput.h>
#include <Engine\UABEngine.h>
#include "KeyboardInput.h"
#include <Utils.h>
#include <dinput.h>

class CDeviceButtonListener;
class CUserButtonListener;

class CInputManager
{
protected:
	gainput::InputManager* m_Manager;
	gainput::InputMap* m_Map;

	float m_AxisX;
	float m_AxisY;
	float m_AxisZ;
	float m_Speed;
	CKeyboardInput* m_KeyBoard;
	bool m_Focus;
	std::string m_Filename;

public:
	enum Actions
	{
		LeftClick,
		RightClick,
		MiddleClick,
		Mouse3,
		Mouse4,
		Mouse5,
		Mouse6,
		WheelUp,
		WheelDown,
		AxisX,
		AxisY,
		MoveForward,
		MoveBackward,
		StrafeLeft,
		StrafeRight,
		Jump,
		Crouch,
		Run,
		Interact,
		Sing,
		Pause,
#ifdef _DEBUG
		DebugToggleFrustum,
		DebugSpeedUp,
		DebugSpeedDown,
		DebugSanityUp,
		DebugSanityDown,
		DebugReloadLua,
		DebugToggleRenderLights,
		DebugToggleRenderAStar,
		DebugToggleLoadVideo,
		DebugConsole,
		DebugChangeCameraVision,
		DebugChangeCamera,
		DebugToggleRenderCamera,
		DebugMusicVolumeUp,
		DebugMusicVolumeDown,
		DebugFxVolumeUp,
		DebugFxVolumeDown,
		DebugMonsterRun,
		DebugMonsterIdle,
		DebugMonsterHit,
#endif
	};

	const gainput::DeviceId* m_KeyboardId;
	const gainput::DeviceId* m_MouseId;
	const gainput::DeviceId* m_GamepadId;

	CInputManager();
	~CInputManager();
	void Update();

	void Load(std::string _file);
	void Reload();
	void SetWindow(HWND _hWnd, int _width, int _height);
	int GetAction(std::string _name);
	int GetInput(std::string _name);

	void SetFocus(bool _focus);
	bool GetFocus() const;

	void UpdateAxis(LONG _x, LONG _y);
	Vect2f GetCursor();
	Vect2f GetCursorMovement();
	float GetAxisX();
	float GetAxisY();
	bool IsActionActive(std::string _name);
	bool IsActionNew(std::string _name);
	bool IsActionReleased(std::string _name);
	bool WasActionActive(std::string _name);
	CKeyboardInput* GetKeyBoard(){ return m_KeyBoard; };
	gainput::InputManager* GetManager(){ return m_Manager; };
	gainput::InputMap* GetMap(){ return m_Map; };
};

#endif