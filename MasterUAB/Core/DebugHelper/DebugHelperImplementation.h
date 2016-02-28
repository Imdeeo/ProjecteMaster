#ifndef DEBUGHELPERIMPLEMENTATION_H
#define DEBUGHELPERIMPLEMENTATION_H

#include <Windows.h>
#include <unordered_map>
#include <AntTweakBar.h>
#include "DebugHelper.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

class CDebugHelperImplementation : public CDebugHelper
{

private:
	std::vector<std::string>				m_ButtonLuaScripts;

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
	void AddVariable(const std::string &_VarName, const EDebugType _type, const Mode _mode = READ_WRITE, void* _pointer = nullptr);

	
	std::unordered_map<std::string, TwBar*> m_Bars;
	TwType m_PosRotType;
	UAB_BUILD_GET_SET(std::string, Log);

	static void __stdcall RunLuaButton(void *Data);
};

#endif