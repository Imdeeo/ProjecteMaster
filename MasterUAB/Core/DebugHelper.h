#pragma once

#include <string>
#include <vector>

#include "Utils.h"
#include "HelperTypes.h"
#include "Math\Color.h"

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

public:

	virtual void Render() {};

	virtual void Log(const std::string& text) const {};

	virtual void RegisterBar(const SDebugBar& bar) {};
	virtual void RemoveBar(const std::string& bar) {};

	static CDebugHelper* GetDebugHelper();
	static void SetCurrentDebugHelper(CDebugHelper* _DebugHelper);

protected:
	CDebugHelper() {};
};


