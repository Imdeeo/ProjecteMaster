#include "ScriptedComponent.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include "Engine\UABEngine.h"
#include "RenderManager\RenderManager.h"
#include "ScriptManager\ScriptManager.h"
#include "RenderableObjects\RenderableObject.h"

using namespace luabind;

CScriptedComponent::CScriptedComponent(const std::string &Name, CRenderableObject *Owner, const	std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate,
	const std::string &FnOnRender, const std::string &FnOnRenderDebug) : CUABComponent(Name, Owner)
{
	m_FnOnCreate = FnOnCreate;
	m_FnOnDestroy = FnOnDestroy;
	m_FnOnUpdate = FnOnUpdate;
	m_FnOnRender = FnOnRender;
	m_FnOnRenderDebug = FnOnRenderDebug;

	try
	{
		if (!m_FnOnCreate.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(), m_FnOnCreate.c_str(), GetOwner());
	}
	catch (const luabind::error e)
	{
		
	}
}

CScriptedComponent::~CScriptedComponent()
{
	try
	{
		if (!m_FnOnDestroy.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(), m_FnOnDestroy.c_str(), GetOwner());
	}
	catch (const luabind::error e)
	{

	}
}

void CScriptedComponent::Update(float ElapsedTime)
{
	try
	{
		if(!m_FnOnUpdate.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(),m_FnOnUpdate.c_str(), GetOwner(), ElapsedTime);
	}
	catch(const luabind::error e)
	{
		
	}
}

void CScriptedComponent::Render(CRenderManager &RenderManager)
{
	try
	{
		if (!m_FnOnRender.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(), m_FnOnRender.c_str(), GetOwner(), &RenderManager);
	}
	catch (const luabind::error e)
	{

	}
}

void CScriptedComponent::RenderDebug(CRenderManager &RenderManager)
{
	try
	{
		if (!m_FnOnRenderDebug.empty())
			luabind::call_function<void>(UABEngine.GetScriptManager()->GetLuaState(), m_FnOnRenderDebug.c_str(), GetOwner(), &RenderManager);
	}
	catch (const luabind::error e)
	{

	}
}

CScriptedComponent * CScriptedComponent::CreateScriptedComponent(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy,
	const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug)
{
	return new CScriptedComponent(Name, Owner, FnOnCreate, FnOnDestroy, FnOnUpdate, FnOnRender, FnOnRenderDebug);
}