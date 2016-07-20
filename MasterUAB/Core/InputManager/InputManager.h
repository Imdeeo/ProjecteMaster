#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <gainput\gainput.h>
#include <Engine\UABEngine.h>
#include <Utils.h>
#include "KeyboardInput.h"
#include "Math\Vector2.h"

class CDeviceButtonListener;
class CUserButtonListener;

class CInputManager
{
protected:
	gainput::InputManager* m_Manager;
	gainput::InputMap* m_Map;
	CDeviceButtonListener* m_DeviceButtonListener;
	CDeviceButtonListener* m_DeviceButtonListener2;
	CUserButtonListener* m_UserButtonListener;
	CUserButtonListener* m_UserButtonListener2;
	gainput::ListenerId m_DeviceButtonListenerId;
	gainput::ListenerId m_UserButtonListenerId;

	bool m_Focus;

public:
	enum Button
	{
		ButtonToggleListener,
		ButtonToggleMapListener,
		ButtonConfirm,
		ButtonMouseX,
	};

	gainput::DeviceId m_KeyboardId;
	gainput::DeviceId m_MouseId;

	CInputManager(HWND _hWnd, int _width, int _height);
	~CInputManager();
	void Update();

	gainput::InputManager* GetManager(){ return m_Manager; }

	void SetFocus(bool _focus){ m_Focus = _focus; }
	bool HasFocus() const { return m_Focus; }

	/*bool IsActionActive(const std::string& action) const { return m_ActiveActions.count(action) > 0; }
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