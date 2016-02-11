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

public:
	CDebugHelperImplementation(void *device);
	~CDebugHelperImplementation();
	ofstream m_LogFile;

	bool Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Render();

	virtual void Log(const std::string& text);

	virtual void RegisterBar(const SDebugBar& bar);
	virtual void RemoveBar(const std::string& bar);

	
	std::unordered_map<std::string, TwBar*> m_Bars;
	TwType m_PosRotType;
	UAB_BUILD_GET_SET(std::string, Log);
};

#endif