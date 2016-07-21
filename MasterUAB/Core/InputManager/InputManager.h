#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <string>
#include <gainput\gainput.h>
#include <Engine\UABEngine.h>
#include <Utils.h>

class CDeviceButtonListener;
class CUserButtonListener;

class CInputManager
{
protected:
	gainput::InputManager* m_Manager;
	gainput::InputMap* m_Map;

	bool m_Focus;

public:
	enum Actions
	{
		LeftClick,
		RightClick,
		MiddleClick,
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
		Pause,
#ifdef _DEBUG
		DebugToggleFrustum,
		DebugSpeedUp,
		DebugSpeedDown,
		DebugSanityUp,
		DebugSanityDown,
		DebugReloadLua,
		DebugToggleRenderLights,
		DebugChangeCameraControl,
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

	CInputManager();
	~CInputManager();
	void Update();
	void SetWindow(HWND _hWnd, int _width, int _height);

	gainput::InputManager* GetManager(){ return m_Manager; }
	gainput::InputMap* GetMap(){ return m_Map; }
	void LoadLayout(std::string _file);
	
	/*void SetFocus(bool _focus){ m_Focus = _focus; }
	bool HasFocus() const { return m_Focus; }

	bool IsActionActive(const std::string& action) const { return m_ActiveActions.count(action) > 0; }
	float GetAxis(const std::string& axis) const { std::unordered_map<std::string, float>::const_iterator it = m_ActiveAxis.find(axis); if (it == m_ActiveAxis.end()) return 0; else return it->second; }
	
	Vect2i GetCursor() const { return m_Cursor; }
	Vect2i GetCursorMovement() const { return m_CursorD; }
	CKeyboardInput* GetKeyboard() const { return m_Keyboard; }

	static CInputManager* GetInputManager();
	static void SetCurrentInputManager(CInputManager* _InputManager);
	
	virtual void reload();
	
protected:
	std::unordered_set<std::string> m_ActiveActions;
	std::unordered_map<std::string, float> m_ActiveAxis;
	Vect2i m_Cursor;
	Vect2i m_CursorD;
	CKeyboardInput* m_Keyboard;*/

};

#endif