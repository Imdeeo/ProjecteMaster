#include "ScriptManager.h"
#include <assert.h>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "AnimatedModels\AnimatedCoreModel.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "AnimatedModels\AnimatedModelsManager.h"

using namespace luabind;

CScriptManager g_ScriptManager;

#define LUA_STATE CUABEngine::GetInstance().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

CScriptManager::CScriptManager()
: m_LS(NULL)
{
}

//Código de la función Alert que se llamará al generarse algún error de LUA
int Alert(lua_State * State)
{
	std::string l_Text;
	int n = lua_gettop(State);
	int i;
	lua_getglobal(State, "tostring");
	for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(State, -1);
		lua_pushvalue(State, i);
		lua_call(State, 1, 1);
		s = lua_tostring(State, -1);
		if (s == NULL)
			return luaL_error(State, "`tostring' must return a string to `print'");
		if (i>1) l_Text += '\t';
		l_Text += s;
		lua_pop(State, 1);
	}
	l_Text += '\n';
	//Info( l_Text.c_str() );
	assert(!"must be log");
	return true;
}

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
	m_LS=lua_open();
	luaopen_base(m_LS);
	luaopen_string(m_LS);
	luaopen_table(m_LS);
	luaopen_math(m_LS);
	luaopen_debug(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA 
	lua_register(m_LS,"_ALERT",Alert);

	luabind::open(m_LS);

	RegisterLUAFunctions();
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
	lua_close(m_LS);
}

//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		//Info("%s",l_Str);
		assert(!"must be log");
	}
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		//Info("%s",l_Str);
		assert(!"must be log");
	}
}

void CScriptManager::RegisterLUAFunctions()
{
	//lua_register(m_LS, "set_speed_player", SetSpeedPlayer);
	/*lua_register(m_LS, "set_speed_player", SetSpeedPlayer);
	lua_register(m_LS, "get_speed_player", GetSpeedPlayer);*/
	//REGISTER_LUA_FUNCTION("set_speed_player", SetSpeedPlayer);
	//REGISTER_LUA_FUNCTION("get_speed_player", GetSpeedPlayer);

	module(m_LS) [
		class_<CAnimatedCoreModel>("CAnimatedCoreModel")
			.def(constructor<>())
			.def("get_name", &CAnimatedCoreModel::GetName)
			.def("set_name", &CAnimatedCoreModel::SetName)
			.def("get_calCoreModel", &CAnimatedCoreModel::GetCalCoreModel)
			.def("load",&CAnimatedCoreModel::Load)
	];
	module(m_LS) [
		class_<CAnimatedInstanceModel>("CAnimatedInstanceModel")
			.def(constructor<>())
			.def("get_name", &CAnimatedInstanceModel::GetName)
			.def("set_name", &CAnimatedInstanceModel::SetName)
			.def("initialize",&CAnimatedInstanceModel::Initialize)
			.def("render",&CAnimatedInstanceModel::Render)
			.def("update",&CAnimatedInstanceModel::Update)
			.def("destroy",&CAnimatedInstanceModel::Destroy)
			.def("execute_action",&CAnimatedInstanceModel::ExecuteAction)
			.def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
			.def("clera_cycle",&CAnimatedInstanceModel::ClearCycle)
			.def("is_cycle_animation_active",&CAnimatedInstanceModel::IsCycleAnimationActive)
			.def("is_action_animation_active",&CAnimatedInstanceModel::IsActionAnimationActive)
	];

	module(m_LS) [
		class_<CAnimatedModelsManager>("CAnimatedModelsManager")
			.def(constructor<>())
			.def("load",&CAnimatedModelsManager::Load)
			.def("reload", &CAnimatedModelsManager::Reload)
			.def("get_resource", &CAnimatedModelsManager::GetResource)
	];
	
	//RunFile("./data/scripting/init.lua");

	//RunCode("Init()");
	//RunCode("local value=3;set_speed_player(value); value=get_speed_player()+2; set_speed_player(value)");
	//RunCode("local animatedcoremodel=CAnimatedCoreModel(); animatedcoremodel:set_name(\"Nombre\"); local name=animatedcoremodel:get_name()");
}

/*void OnEnterEvent(CEvent *Event)
{
	RunCode("on_enter_player_on_event_"+Event->GetName()+"()");
	RunCode("on_enter_player_on_event_event_1()");
}*/
