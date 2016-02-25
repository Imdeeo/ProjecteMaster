#include "ScriptedComponent.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include "Engine\UABEngine.h"

using namespace luabind;

void CScriptedComponent::Update(float ElapsedTime)
{
	try
	{
		if(!m_FnOnUpdate.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(),m_FnOnUpdate.c_str(), GetOwner(), ElapsedTime);
	}
	catch(const luabind::error &e)
	{
		
	}
}