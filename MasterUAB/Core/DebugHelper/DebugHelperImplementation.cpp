#include "DebugHelperImplementation.h"

#include "Engine\UABEngine.h"
#include <cassert>
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>  

CDebugHelperImplementation::CDebugHelperImplementation(void *device)
{
	// TODO: inicializar AntTweakBar
	
	int status = TwInit(TW_DIRECT3D11, device);
	assert(status);

	{
		TwStructMember structMembers[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, x), "step=0.5 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, y), "step=0.5 precision=1" },
			{ "z", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, z), "step=0.5 precision=1" },
			{ "yaw", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Yaw), "step=0.05 precision=2" },
			{ "pitch", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Pitch), "step=0.05 precision=2" },
			{ "roll", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Roll), "step=0.05 precision=2" }
		};

		m_PosRotType = TwDefineStruct("POSITION_ORIENTATION", structMembers, 6, sizeof(SPositionOrientation), nullptr, nullptr);
	}
	m_Log = "";
}


CDebugHelperImplementation::~CDebugHelperImplementation()
{
	// TODO: finalizar AntTweakBar
	std::filebuf l_LogFile;
	l_LogFile.open("Data\\LogFile.txt", std::ios::out);
	std::ostream os(&l_LogFile);
	os << std::string(m_Log);
	l_LogFile.close();
	int status = TwTerminate();
	assert(status);
	
}

void CDebugHelperImplementation::Log(const std::string& text) 
{
	// TODO: OPCIONAL esto va bién tenerlo escribiendo en un fichero
	//OutputDebugString(("LOG: " + text).c_str());
	m_Log = m_Log + text + "\n";
}

void CDebugHelperImplementation::Render()
{
	// TODO: pintar el AntTweakBar
	
	int status = TwDraw();
	if (!status)
	{
		const char* error = TwGetLastError();
		Log(error);
	}
	
}

bool CDebugHelperImplementation::Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: mandarle eventos al AntTweakBar
	return TwEventWin(hWnd, msg, wParam, lParam)!=0;
}

void CDebugHelperImplementation::StartRegisterBar(std::string _BarName)
{
	m_ActualBar = SDebugBar();
	m_ActualBar.name = _BarName;
}

void CDebugHelperImplementation::RegisterBar()
{
	int status = 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(m_ActualBar.name);
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
	}

	TwBar* twBar = TwNewBar(m_ActualBar.name.c_str());

	for (size_t i = 0; i < m_ActualBar.variables.size(); ++i)
	{
		if (m_ActualBar.variables[i].type == BUTTON)
		{
			status = TwAddButton(twBar, m_ActualBar.variables[i].name.c_str(), m_ActualBar.variables[i].callback, m_ActualBar.variables[i].data, m_ActualBar.variables[i].definition.c_str());
			assert(status);
		}
		else
		{
			TwType type = TW_TYPE_FLOAT;
			std::string params = m_ActualBar.variables[i].definition;
			switch (m_ActualBar.variables[i].type)
			{
			case BOOL:
				type = TW_TYPE_BOOLCPP;
				break;
			case FLOAT:
				type = TW_TYPE_FLOAT;
				break;
			case INT:
				type = TW_TYPE_INT32;
				break;
			case COLOR32:
				type = TW_TYPE_COLOR32;
				params = params + " coloralpha=true colororder=argb ";
				break;
			case COLOR:
				type = TW_TYPE_COLOR4F;
				break;
			case STRING:
				type = TW_TYPE_CSSTRING(100);
				break;

			case POSITION_ORIENTATION:
				type = m_PosRotType;
				break;

			default:
				break;
			}

			switch (m_ActualBar.variables[i].mode)
			{
			case READ:
				status = TwAddVarRO(twBar, m_ActualBar.variables[i].name.c_str(), type, m_ActualBar.variables[i].ptr, params.c_str());
				assert(status);
				break;

			case READ_WRITE:
				status = TwAddVarRW(twBar, m_ActualBar.variables[i].name.c_str(), type, m_ActualBar.variables[i].ptr, params.c_str());
				assert(status);
				break;

			default:
				break;
			}
		}
	}

	m_Bars[m_ActualBar.name] = twBar;
	
}

void CDebugHelperImplementation::RemoveBar(std::string _BarName)
{
	TwDeleteBar(m_Bars[_BarName]);
	m_Bars.erase(_BarName);
	m_ButtonLuaScripts.clear();
	m_ActualBar = SDebugBar();
}

void CDebugHelperImplementation::AddLuaButton(const std::string &ButtonName, const std::string &LuaScript, const std::string &ButtonDefinition)
{
	m_ButtonLuaScripts.push_back(LuaScript);
	SDebugVariable l_InfoButton;
	l_InfoButton.callback=RunLuaButton;
	l_InfoButton.data=(void *)&m_ButtonLuaScripts[m_ButtonLuaScripts.size()-1].c_str()[0];
	l_InfoButton.name=ButtonName;
	l_InfoButton.type=BUTTON;
	l_InfoButton.definition=ButtonDefinition;
	m_ActualBar.variables.push_back(l_InfoButton);
}
void CDebugHelperImplementation::AddVariable(const std::string &_VarName, const EDebugType _type, const Mode _mode, CEmptyPointerClass* _pointer, const std::string &_params)
{
	SDebugVariable var = {};
	var.name = _VarName;
	var.type = _type;
	var.mode = _mode;
	var.definition = _params;
	if (_type == BOOL)
	{
		var.pBool = (bool*)_pointer;
	}
	if (_type == FLOAT)
	{
		var.pFloat = (float*)_pointer;
	}
	if (_type == INT)
	{
		var.pInt = (int*)_pointer;
	}
	if (_type == COLOR32)
	{
		var.pColor = (CColor*)_pointer;
	}
	if (_type == COLOR)
	{
		var.pColor32= (uint32*)_pointer;
	}
	if (_type == STRING)
	{
		var.pString = (char*)_pointer;
	}
	if (_type == POSITION_ORIENTATION)
	{
		var.pPositionOrientation = (SPositionOrientation*)_pointer;
	}
	m_ActualBar.variables.push_back(var);
}

void __stdcall CDebugHelperImplementation::RunLuaButton(void *Data)
{
	UABEngine.GetScriptManager()->RunCode((const char*)Data);
}
