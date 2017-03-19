#ifndef DEBUGHELPERIMPLEMENTATION_H
#define DEBUGHELPERIMPLEMENTATION_H

#include <Windows.h>
#include <unordered_map>
#include "DebugHelper.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

typedef struct CTwBar TwBar;
typedef enum ETwType TwType;

class CDebugHelperImplementation : public CDebugHelper
{

private:
	std::vector<std::string>				m_ButtonLuaScripts;
	std::string m_Log;

public:
	CDebugHelperImplementation(void *device);
	~CDebugHelperImplementation();
	ofstream m_LogFile;

	bool Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Render();

	virtual void Log(const std::string& text);

	virtual void StartRegisterBar(std::string _BarName);
	virtual void RegisterBar();
	virtual void RemoveBar(std::string _BarName);
	void AddLuaButton(const std::string &ButtonName, const std::string &LuaScript, const std::string &ButtonDefinition);
	void AddVariable(const std::string &_VarName, const EDebugType _type, const Mode _mode = READ_WRITE, CEmptyPointerClass* _pointer = nullptr, const std::string &_params = "");

	
	std::unordered_map<std::string, TwBar*> m_Bars;
	TwType* m_PosRotType;
	
	void SetLog(std::string _Log){ m_Log = _Log; }
	std::string GetLog(){ return m_Log; }

	static void __stdcall RunLuaButton(void *Data);
};

#endif