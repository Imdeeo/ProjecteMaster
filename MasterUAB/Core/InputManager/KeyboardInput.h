#ifndef H_KEYBOARD_INPUT_H
#define H_KEYBOARD_INPUT_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Math\Vector2.h"

class CKeyboardInput
{
private:
	char m_LastChar;
	bool m_ActiveLastChar;
public:
	CKeyboardInput();
	~CKeyboardInput(){};
	void SetLastChar(char _caracter);
	char ConsumeLastChar();
};

#endif //H_KEYBOARD_INPUT_H