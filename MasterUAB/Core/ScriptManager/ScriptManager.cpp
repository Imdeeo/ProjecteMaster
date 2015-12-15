#include "ScriptManager.h"
#include <assert.h>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "AnimatedModels\AnimatedCoreModel.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "AnimatedModels\AnimatedModelsManager.h"

#include "Camera\Camera.h"
#include "Camera\CameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\Frustum.h"
#include "Camera\SphericalCameraController.h"

#include "XML\XMLTreeNode.h"

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

	// AnimatedModels----------------------------------------------------------------------------------
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
			.def(constructor<CXMLTreeNode&>())
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
	

	// Camera----------------------------------------------------------------------------------------
	module(m_LS) [
		class_<CCamera>("CCamera")
			.def(constructor<>())
			.def("get_aspect_ratio",&CCamera::GetAspectRatio)
			.def("set_aspect_ratio",&CCamera::SetAspectRatio)
			.def("get_fov", &CCamera::GetFOV)
			.def("set_fov", &CCamera::SetFOV)
			.def("get_look_at", &CCamera::GetLookAt)
			.def("set_look_at", &CCamera::SetLookAt)
			.def("get_position", &CCamera::GetPosition)
			.def("set_position", &CCamera::SetPosition)
			.def("get_projection", &CCamera::GetProjection)
			.def("get_up", &CCamera::GetUp)
			.def("set_up", &CCamera::SetUp)
			.def("get_view", &CCamera::GetView)
			.def("get_z_far", &CCamera::GetZFar)
			.def("set_z_far", &CCamera::SetZFar)
			.def("get_z_near", &CCamera::GetZNear)
			.def("set_z_near", &CCamera::GetZNear)
			.def("set_matrixs", &CCamera::SetMatrixs)
	];

	module(m_LS) [
		class_<CFPSCameraController>("CFPSCameraController")
			.def(constructor<>())
			.def("add_pitch",&CFPSCameraController::AddPitch)
			.def("add_yaw", &CFPSCameraController::AddYaw)
			.def("get_direction", &CFPSCameraController::GetDirection)
			.def("get_pitch", &CFPSCameraController::GetPitch)
			.def("set_pitch", &CFPSCameraController::SetPitch)
			.def("get_right", &CFPSCameraController::GetRight)
			.def("get_up", &CFPSCameraController::GetUp)
			.def("get_yaw", &CFPSCameraController::GetYaw)
			.def("set_yaw", &CFPSCameraController::SetYaw)
			.def("move", &CFPSCameraController::Move)
			.def("set_camera", &CFPSCameraController::SetCamera)
			.def("set_position", &CFPSCameraController::SetPosition)
			.def("update", &CFPSCameraController::Update)
	];

	module(m_LS) [
		class_<CFrustum>("CFrustum")
			.def(constructor<>())
			.def("box_visible",&CFrustum::BoxVisible)
			.def("sphere_visible", &CFrustum::SphereVisible)
			.def("update", &CFrustum::Update)
	];

	module(m_LS) [
		class_<CSphericalCameraController>("CSphericalCameraController")
			.def(constructor<>())
			.def("add_pitch",&CSphericalCameraController::AddPitch)
			.def("add_yaw", &CSphericalCameraController::AddYaw)
			.def("add_zoom", &CSphericalCameraController::AddZoom)
			.def("get_direction", &CSphericalCameraController::GetDirection)
			.def("set_camera", &CSphericalCameraController::SetCamera)
			.def("get_pitch", &CSphericalCameraController::GetPitch)
			.def("set_pitch", &CSphericalCameraController::SetPitch)
			.def("get_right", &CSphericalCameraController::GetRight)
			.def("get_up", &CSphericalCameraController::GetUp)
			.def("get_yaw", &CSphericalCameraController::GetYaw)
			.def("set_yaw", &CSphericalCameraController::SetYaw)
			.def("set_position", &CSphericalCameraController::SetPosition)
			.def("set_zoom", &CSphericalCameraController::SetZoom)
			.def("update", &CSphericalCameraController::Update)
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

