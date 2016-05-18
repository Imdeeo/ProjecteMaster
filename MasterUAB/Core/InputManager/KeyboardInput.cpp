#include "KeyboardInput.h"

CKeyboardInput::CKeyboardInput()
{
	m_ActiveLastChar = false;
	m_LastChar = 'a';
}

void CKeyboardInput::SetLastChar(char _caracter)
{
	m_LastChar = _caracter;
	m_ActiveLastChar = true;
}

char CKeyboardInput::ConsumeLastChar()
{
	m_ActiveLastChar = false;
	return m_LastChar;
}