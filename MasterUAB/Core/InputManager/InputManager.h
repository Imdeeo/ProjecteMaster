#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "KeyboardInput.h"
#include "Math\Vector2.h"

class CInputManager
{
public:

	bool IsActionActive(const std::string& action) const { return m_ActiveActions.count(action) > 0; }
	float GetAxis(const std::string& axis) const { std::unordered_map<std::string, float>::const_iterator it = m_ActiveAxis.find(axis); if (it == m_ActiveAxis.end()) return 0; else return it->second; }
	
	Vect2i GetCursor() const { return m_Cursor; }
	Vect2i GetCursorMovement() const { return m_CursorD; }
	CKeyboardInput* GetKeyboard() const { return m_Keyboard; }
	bool HasFocus() const { return m_Focus; }

	static CInputManager* GetInputManager();
	static void SetCurrentInputManager(CInputManager* _InputManager);
	
	virtual void reload();
	
protected:


	CInputManager() :m_Cursor(0, 0), m_CursorD(0, 0), m_Focus(true) {}

	std::unordered_set<std::string> m_ActiveActions;
	std::unordered_map<std::string, float> m_ActiveAxis;
	Vect2i m_Cursor;
	Vect2i m_CursorD;
	CKeyboardInput* m_Keyboard;

	bool m_Focus;

};

#endif