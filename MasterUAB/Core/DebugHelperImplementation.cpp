#include "DebugHelperImplementation.h"

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

void CDebugHelperImplementation::RegisterBar(const SDebugBar& bar)
{
	// TODO: registrar una ventana de debug
	
	int status = 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(bar.name);
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
	}

	TwBar* twBar = TwNewBar(bar.name.c_str());

	for (size_t i = 0; i < bar.variables.size(); ++i)
	{
		if (bar.variables[i].type == BUTTON)
		{
			status = TwAddButton(twBar, bar.variables[i].name.c_str(), bar.variables[i].callback, bar.variables[i].data, "");
			assert(status);
		}
		else
		{
			TwType type = TW_TYPE_FLOAT;
			const char* params = "";
			switch (bar.variables[i].type)
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
				params = " coloralpha=true colororder=argb ";
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

			switch (bar.variables[i].mode)
			{
			case READ:
				status = TwAddVarRO(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params);
				assert(status);
				break;

			case READ_WRITE:
				status = TwAddVarRW(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params);
				assert(status);
				break;

			default:
				break;
			}
		}
	}

	m_Bars[bar.name] = twBar;
	
}

void CDebugHelperImplementation::RemoveBar(const std::string& bar)
{
	// TODO: eliminar una ventana de debug
	TwDeleteBar(m_Bars[bar]);
	m_Bars.erase(bar);
	
}
