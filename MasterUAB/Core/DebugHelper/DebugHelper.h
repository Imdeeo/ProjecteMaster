#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H


#include "Utils.h"
#include "HelperTypes.h"
#include "Math\Color.h"

#include <map>
#include <string>

class CDebugHelper
{
public:

	enum EDebugType
	{
		BUTTON,

		BOOL,
		FLOAT,
		INT,
		COLOR32,
		COLOR,
		STRING,

		// other data types
		POSITION_ORIENTATION


	};


	enum Mode
	{
		READ,
		READ_WRITE
	};

	typedef void __stdcall Callback(void*data);

	struct SDebugVariable
	{
		std::string name;
		std::string definition;
		EDebugType type;
		union {
			Mode mode;
			Callback *callback;
		};
		union {
			bool *pBool;
			float *pFloat;
			int *pInt;
			uint32_t *pColor32;
			CColor *pColor;
			char* pString; // [100]
			SPositionOrientation *pPositionOrientation;

			void* data;

			void* ptr;
		};
	};

	struct SDebugBar
	{
		std::string name;
		std::vector<SDebugVariable> variables;
	};
protected:
	SDebugBar m_ActualBar;
public:

	virtual void Render() {};

	virtual void Log(const std::string& text) {};

	virtual void StartRegisterBar(std::string _BarName){};
	virtual void RegisterBar() {};
	virtual void RemoveBar(std::string _BarName) {};

	virtual void AddLuaButton(const std::string &ButtonName, const std::string &LuaScript, const std::string &ButtonDefinition){};
	virtual void AddVariable(const std::string &_VarName, const EDebugType _type, const Mode _mode, void* _pointer){}

	static CDebugHelper* GetDebugHelper();
	static void SetCurrentDebugHelper(CDebugHelper* _DebugHelper);

protected:
	CDebugHelper() {};
};

#endif