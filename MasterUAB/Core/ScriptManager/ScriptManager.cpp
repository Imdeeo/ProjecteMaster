#include "ScriptManager.h"
#include <assert.h>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "3DElement\3DElement.h"

#include "Utils\CEmptyPointerClass.h"

#include "Utils\Named.h"
#include "Utils\Active.h"
#include "Components\UABComponent.h"
#include "Components\UABComponentManager.h"
#include "Components\ScriptedComponent.h"
#include "Components\LUAComponent.h"

#include "Engine\UABEngine.h"

#include "LevelManager\LevelManager.h"

#include "InputManager\InputManager.h"

#include "DebugHelper\DebugHelper.h"
#include "DebugHelper\DebugHelperImplementation.h"

#include "AnimatedModels\AnimatedCoreModel.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "AnimatedModels\AnimatedModelsManager.h"

#include "Camera\Camera.h"
#include "Camera\CameraController.h"
#include "Camera\CameraControllerManager.h"
#include "Camera\CameraInfo.h"
#include "Camera\CameraKey.h"
#include "Camera\CameraKeyController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\Frustum.h"
#include "Camera\FocusedCameraController.h"
#include "Camera\SphericalCameraController.h"
#include "Camera\3PersonCameraController.h"

#include "Cinematics\CinematicManager.h"
#include "Cinematics\Cinematic.h"
#include "Cinematics\CinematicObject.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include "Cinematics\CinematicPlayer.h"

#include "ContextManager\ContextManager.h"

#include "Effects\AnimatedModelEffectParameters.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectParameters.h"
#include "Effects\EffectShader.h"
#include "Effects\EffectTechnique.h"
#include "Effects\LightEffectParameters.h"
#include "Effects\SceneEffectParameters.h"

#include "Layers\LayerManager.h"

#include "Lights\DirectionalLight.h"
#include "Lights\Light.h"
#include "Lights\LightManager.h"
#include "Lights\OmniLight.h"
#include "Lights\SpotLight.h"

#include "Materials\Material.h"
#include "Materials\MaterialManager.h"

#include "RenderableObjects\RenderableObject.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableVertexs.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\TemplatedRenderableIndexedVertexs.h"
#include "RenderableObjects\TemplatedRenderableVertexs.h"
#include "RenderableObjects\VertexTypes.h"

#include "RenderManager\RenderManager.h"

#include "StaticMesh\InstanceMesh.h"
#include "StaticMesh\StaticMesh.h"
#include "StaticMesh\StaticMeshManager.h"

#include "SceneRender\SceneRendererCommandManager.h"
#include "SceneRender\SceneRendererCommand.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Particles\ParticleManager.h"
#include "Particles\ParticleSystemType.h"
#include "Particles\ParticleSystemInstance.h"
#include "Bilboards\BilboardManager.h"
#include "Bilboards\BilboardSystemInstance.h"
#include "Bilboards\BilboardSystemType.h"
#include "LineRenderer\LineRenderer.h"

#include "Manchas\ManchasManager.h"
#include "Manchas\ManchasSystemType.h"
#include "Manchas\ManchasSystemInstance.h"

#include "GUIManager.h"
#include "GUIPosition.h"
#include "SliderResult.h"

#include "DebugRender.h"

#include "PhysXManager\PhysXManager.h"

#include "SoundManager\SoundManager.h"
#include "VideoManager\VideoManager.h"

#include "IA\AStar.h"
#include "IA\AStarManager.h"

#include "Application.h"
#include "GamePlayManager.h"

#include "XML\tinyxml2.h"
#include "Utils.h"

#include <string>
#include <vector>
#include <map>

#include <typeinfo.h>

#include <cal3d\cal3d.h>

using namespace luabind;

#define LUA_STATE CUABEngine::GetInstance().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

CUABComponent* CreateScriptedComponent(const std::string &Name, CRenderableObject *Owner, std::string _FnOnCreate, std::string _FnOnDestroy, std::string _FnOnUpdate, std::string _FnOnRender, std::string _FnOnRenderDebug)
{
	CUABComponent* l_component = new CScriptedComponent(Name, Owner, _FnOnCreate, _FnOnDestroy,  _FnOnUpdate, _FnOnRender, _FnOnRenderDebug);
	return l_component;
}

int ShowLuaErrorDebugInfo(lua_State* L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;
	if (d.name != 0)
	{
	msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " " << err;
	CDebugHelper::GetDebugHelper()->Log(("Error: %s", msg.str().c_str()));
	return 0;
}

CScriptManager::CScriptManager()
: m_LS(NULL)
{
}

CScriptManager::~CScriptManager()
{
	Destroy();
}

struct CLUAComponent_wrapper : CLUAComponent, luabind::wrap_base
{
	CLUAComponent_wrapper(const std::string &Name, CRenderableObject *Owner)
		: CLUAComponent(Name, Owner)
	{}

	virtual void Update(float _ElapsedTime)
	{
		call<void>("Update", _ElapsedTime);
	}

	static void default_Update(CLUAComponent* ptr, float _ElapsedTime)
	{
		return ptr->CLUAComponent::Update(_ElapsedTime);
	}
};

//C�digo de la funci�n Alert que se llamar� al generarse alg�n error de LUA
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
	//Sobreescribimos la funci�n _ALERT de LUA cuando se genere alg�n error al ejecutar c�digo LUA 
	lua_register(m_LS,"_ALERT",Alert);

	luabind::open(m_LS);

	RegisterLUAFunctions();
	lua_atpanic(m_LS, ShowLuaErrorDebugInfo);
	luabind::set_pcall_callback(ShowLuaErrorDebugInfo);
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
	if(m_LS)
	{
		lua_close(m_LS);
	}
}


//Para ejecutar un fragmento de c�digo LUA
void CScriptManager::RunCode(const std::string &Code) const
{
	//UtilsLog("RunCode");
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		UtilsLog(l_Str);
		//assert(!"must be log");
	}
}

//Para ejecutar un fichero de c�digo LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		UtilsLog(l_Str);
		//assert(!"must be log");
	}
}

template <class T>
void RegisterTemplatedMapManager(lua_State* _LS)
{
	std::string name = "CTemplatedMapManager<" + std::string(typeid(T).name()) + ">";
	module(_LS)[
		class_<CTemplatedMapManager<T>>(name.c_str())
			.def("get_resource", &CTemplatedMapManager<T>::GetResource)
			.def("add_resource", &CTemplatedMapManager<T>::AddResource)
			.def("destroy", &CTemplatedMapManager<T>::Destroy)
			.def("size", &CTemplatedMapManager<T>::Size)
			.def("get_map", &CTemplatedMapManager<T>::GetResourcesMap, luabind::return_stl_iterator)
			.def("get_elements_array", &CTemplatedMapManager<T>::GetElementsArray)
	];
}

template <class T>
void RegisterTemplatedVectorMapManager(lua_State* _LS)
{
	std::string name = "CTemplatedVectorMapManager<" + std::string(typeid(T).name()) + ">";
	module(_LS)[
		class_<CTemplatedVectorMapManager<T>>(name.c_str())
			.def("get_resource", &CTemplatedVectorMapManager<T>::GetResource)
			.def("get_resource_by_id", &CTemplatedVectorMapManager<T>::GetResourceById)
			.def("add_resource", &CTemplatedVectorMapManager<T>::AddResource)
			.def("destroy", &CTemplatedVectorMapManager<T>::Destroy)
			.def("size", &CTemplatedVectorMapManager<T>::Size)
			.def("get_map", &CTemplatedVectorMapManager<T>::GetResourcesMap, luabind::return_stl_iterator)
			.def("get_vector", &CTemplatedVectorMapManager<T>::GetResourcesVector, luabind::return_stl_iterator)
			.def("get_elements_array", &CTemplatedVectorMapManager<T>::GetElementsArray)
	];
}


void CScriptManager::RegisterLUAFunctions()
{
	//RegisterTemplatedVector<int>(m_LS);
	//lua_register(m_LS, "set_speed_player", SetSpeedPlayer);
	/*lua_register(m_LS, "set_speed_player", SetSpeedPlayer);
	lua_register(m_LS, "get_speed_player", GetSpeedPlayer);*/
	//REGISTER_LUA_FUNCTION("set_speed_player", SetSpeedPlayer);
	//REGISTER_LUA_FUNCTION("get_speed_player", GetSpeedPlayer);

// String

	/*module(m_LS)[
		class_<std::string>("StdString")
	];*/

	// Utils-------------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CNamed>("CNamed")
			.def(constructor<tinyxml2::XMLElement*>())
			.def(constructor<const std::string&>())
			.def("get_name_address",&CNamed::GetLuaNameAddress)
			.property("name", &CNamed::GetName, &CNamed::SetName)	];

	module(m_LS)[
		class_<CActive>("CActive")
			.def(constructor<tinyxml2::XMLElement*>())
			.def(constructor<bool>())
			.property("active", &CActive::GetActive, &CActive::SetActive)
			.def("get_address", &CActive::GetLuaAdress)
	];


	luabind::module(m_LS)[ luabind::def("utils_log", &UtilsLog) ];
	luabind::module(m_LS)[luabind::def("utils_log_v2", &UtilsLogV2)];
	luabind::module(m_LS)[luabind::def("utils_log_v3", &UtilsLogV3)];
	luabind::module(m_LS)[luabind::def("utils_log_q", &UtilsLogQ)];

	module(m_LS)[
		class_<Vect3f>("Vect3f")
			.def(constructor<float, float, float>())
			.def_readwrite("x", &Vect3f::x)
			.def_readwrite("y", &Vect3f::y)
			.def_readwrite("z", &Vect3f::z)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self ^ const_self)
			.def(const_self * other<const float>())
			.def(const_self / other<const float>())
			.def("set", &Vect3f::Set)
			.def("get_absolute", &Vect3f::GetAbsolute)
			.def("normalize", &Vector3<float>::Normalize)
			.def("get_normalized", &Vector3<float>::GetNormalized)
			.def("distance", &Vect3f::Distance)
			.def("length", &Vect3f::Length)
			.def("get_angle_with", &Vect3f::GetAngleWith)
	];

	module(m_LS)[
		class_<Vect2f>("Vect2f")
			.def(constructor<float, float>())
			.def_readwrite("x", &Vect2f::x)
			.def_readwrite("y", &Vect2f::y)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * other<const float>())
			.def(const_self / other<const float>())
			.def("set", &Vect2f::Set)
			.def("length", &Vect2f::Length)
	];

	module(m_LS)[
		class_<Quatf>("Quatf")
			.def(constructor<>())
			.def(constructor<float, float, float, float>())
			.def(constructor<Vect3f, float>())
			.def_readwrite("x", &Quatf::x)
			.def_readwrite("y", &Quatf::y)
			.def_readwrite("z", &Quatf::z)
			.def_readwrite("w", &Quatf::w)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self * other<const float>())
			.def(const_self / other<const float>())
			.def("normalize", &Quatf::normalize)
			.def("decouple_x", &Quatf::decoupleX)
			.def("decouple_y", &Quatf::decoupleY)
			.def("decouple_z", &Quatf::decoupleZ)
			.def("conjugate", &Quatf::conjugate)
			.def("get_yaw", &Quatf::GetYaw)
			.def("get_pitch", &Quatf::GetPitch)
			.def("get_roll", &Quatf::GetRoll)
			.def("get_scaled_axis", &Quatf::GetScaledAxis)
			.def("set_from_scaled_axis", &Quatf::SetFromScaledAxis)
			.def("set_from_angle_axis", &Quatf::SetFromAngleAxis)
			.def("set_from_fwd_up", &Quatf::SetFromFwdUp)
			.def("quat_from_yaw_pitch_roll", &Quatf::QuatFromYawPitchRoll)
			.def("get_forward_vector", &Quatf::GetForwardVector)
			.def("rotated_vector", &Quatf::RotateVectorByQuat)
			.def("rotation_matrix", &Quatf::rotationMatrix)
			.def("slerp", (Quatn<float>(Quatn<float>::*)(const Quatn<float>&,float))&Quatf::slerp)
			.scope[
				def("slerp", (Quatn<float>(*)(const Quatn<float> &,const Quatn<float> &, float))&Quatn<float>::slerp)
			]
			.def("slerpJU", (Quatn<float>(Quatn<float>::*)(const Quatn<float>&, float))&Quatf::slerpJU)
			/*.scope[
				def("slerp", (Quatn<float>(*)(const Quatn<float> &, const Quatn<float> &, float))&Quatn<float>::slerp)
			]*/
			.def("compare", &Quatf::Compare)
	];
	
	module(m_LS)[
		class_<Mat33f>("Mat33f")
			.def(constructor<float, float, float, float, float, float, float, float, float>())
			.def_readwrite("m00", &Mat33f::m00)
			.def_readwrite("m01", &Mat33f::m01)
			.def_readwrite("m02", &Mat33f::m02)
			.def_readwrite("m10", &Mat33f::m10)
			.def_readwrite("m11", &Mat33f::m11)
			.def_readwrite("m12", &Mat33f::m12)
			.def_readwrite("m20", &Mat33f::m20)
			.def_readwrite("m21", &Mat33f::m21)
			.def_readwrite("m22", &Mat33f::m22)
			.def(const_self * other<const Vect3f>())
	];

	module(m_LS)[
		class_<CColor>("CColor")
			.def(constructor<float, float, float, float>())
	];

	// BASE--------------------------------------------------------------------------------------------

	// 3DElement---------------------------------------------------------------------------------------
	module(m_LS)[
		class_<C3DElement>("C3DElement")
			.def(constructor<>())
			.def(constructor<const Vect3f&>())
			.def(constructor<const Vect3f&, const Quatf&>())
			.def(constructor<const Vect3f&, float, float, float>())
			.def(constructor<const Quatf&>())
			.def(constructor<float, float, float>())
			.def(constructor<tinyxml2::XMLElement*>())
			.def("set_position", &C3DElement::SetPosition)
			.def("get_position", &C3DElement::GetPosition)
			.def("get_prev_position", &C3DElement::GetPrevPosition)
			.def("set_rotation", &C3DElement::SetRotation)
			.def("get_rotation", &C3DElement::GetRotation)
			.def("set_scale", &C3DElement::SetScale)
			.def("get_scale", &C3DElement::GetScale)
			.def("render", &C3DElement::Render)
			.def("get_transform", &C3DElement::GetTransform)
			.def("get_visible", &C3DElement::GetVisible)
			.def("set_visible", &C3DElement::SetVisible)
			.def("get_lua_pos_x", &C3DElement::GetPositionX)
			.def("get_lua_pos_y", &C3DElement::GetPositionY)
			.def("get_lua_pos_z", &C3DElement::GetPositionZ)
			.def("reset_animated_values", &C3DElement::ResetAnimatedValues)
	];

	module(m_LS)[
		class_<CUABComponent, CNamed>("CUABComponent")
			.def(constructor<const std::string &, CRenderableObject *>())
			.def("Update", &CUABComponent::Update)
			.def("render", &CUABComponent::Render)
			.def("render_debug", &CUABComponent::RenderDebug)
	];
	
	module(m_LS)[
		class_<CLUAComponent, CLUAComponent_wrapper>("CLUAComponent")
			.def(constructor<const std::string &, CRenderableObject *>())
			.def("Update", &CLUAComponent::Update, &CLUAComponent_wrapper::default_Update)
	];

	luabind::module(m_LS) [ luabind::def("create_scripted_component", &CreateScriptedComponent) ];


	module(m_LS)[
		class_<CEmptyPointerClass>("CEmptyPointerClass")
	];

	module(m_LS)[
		class_<tinyxml2::XMLError>("XMLError")
	];

	module(m_LS)[
		class_<tinyxml2::XMLDocument>("XMLDocument")
			.def(constructor<>())
			.def("load_file", (tinyxml2::XMLError(tinyxml2::XMLDocument::*)(const char*))&tinyxml2::XMLDocument::LoadFile)
			.def("first_child_element", (tinyxml2::XMLElement*(tinyxml2::XMLDocument::*)(const char*))&tinyxml2::XMLDocument::FirstChildElement)
	];

	module(m_LS)[
		class_<tinyxml2::XMLElement>("XMLElement")
			//.def(constructor<>())
			.def("get_name", &tinyxml2::XMLElement::Name)
			.def("get_next", &tinyxml2::XMLElement::NextSiblingElement2)
			.def("get_psz_property", &tinyxml2::XMLElement::GetPszProperty)
			.def("get_vect2f_property", &tinyxml2::XMLElement::GetVect2fProperty)
			.def("get_vect3f_property", &tinyxml2::XMLElement::GetVect3fProperty)
			.def("get_vect4f_property", &tinyxml2::XMLElement::GetVect4fProperty)
			.def("get_float_property", &tinyxml2::XMLElement::GetFloatProperty)
			.def("get_bool_property", &tinyxml2::XMLElement::GetBoolProperty)
			.def("get_quat_property", &tinyxml2::XMLElement::GetQuatfProperty)
			.def("first_child", &tinyxml2::XMLElement::FirstChildElement2)
	];

// CORE---------------------------------------------------------------------------------------------

	module(m_LS)[
		class_<CDebugHelper>("CDebugHelper")
			.def("render", &CDebugHelper::Render)
			.def("log", &CDebugHelper::Log)
			.def("start_register_bar", &CDebugHelper::StartRegisterBar)
			.def("register_bar", &CDebugHelper::RegisterBar)
			.def("remove_bar", &CDebugHelper::RemoveBar)
			.scope[
				def("get_debug_helper", &CDebugHelper::GetDebugHelper)
			]
			.def("add_lua_button",&CDebugHelper::AddLuaButton)
			.def("add_variable", &CDebugHelper::AddVariable)
			.def("set_current_debug_helper", &CDebugHelper::SetCurrentDebugHelper)
			.enum_("e_debug_type")[
				value("button", CDebugHelper::BUTTON),
				value("bool", CDebugHelper::BOOL),
				value("float", CDebugHelper::FLOAT),
				value("int", CDebugHelper::INT),
				value("color_32", CDebugHelper::COLOR32),
				value("color", CDebugHelper::COLOR),
				value("string", CDebugHelper::STRING),
				value("position_orientation", CDebugHelper::POSITION_ORIENTATION)
			]
			.enum_("mode")[
				value("read", CDebugHelper::READ),
				value("read_write", CDebugHelper::READ_WRITE)
			]
			.scope[
				class_<CDebugHelper::SDebugVariable>("SDebugVariable")
					.def_readwrite("name",&CDebugHelper::SDebugVariable::name)
					.def_readwrite("type", &CDebugHelper::SDebugVariable::type)
					.def_readwrite("mode", &CDebugHelper::SDebugVariable::mode)
					//.def_readwrite("callback", (void (_stdcall* CDebugHelper::SDebugVariable::*)(void*))&CDebugHelper::SDebugVariable::callback)
					.def_readwrite("p_bool", &CDebugHelper::SDebugVariable::pBool)
					.def_readwrite("p_float", &CDebugHelper::SDebugVariable::pFloat)
					.def_readwrite("p_int", &CDebugHelper::SDebugVariable::pInt)
					.def_readwrite("p_color_32", &CDebugHelper::SDebugVariable::pColor32)
					.def_readwrite("p_color", &CDebugHelper::SDebugVariable::pColor)
					.def_readwrite("p_string", &CDebugHelper::SDebugVariable::pString)
					.def_readwrite("p_position_orientation", &CDebugHelper::SDebugVariable::pPositionOrientation)
					.def_readwrite("data", &CDebugHelper::SDebugVariable::data)
					.def_readwrite("ptr", &CDebugHelper::SDebugVariable::ptr),

				class_<CDebugHelper::SDebugBar>("SDebugBar")
					.def_readwrite("name",&CDebugHelper::SDebugBar::name)
					.def_readwrite("variables",&CDebugHelper::SDebugBar::variables)
			]
	];

	module(m_LS)[
		class_<CDebugHelperImplementation, CDebugHelper>("CDebugHelperImplementation")
			.def(constructor<void*>())
			.def("update", &CDebugHelperImplementation::Update)
			.def("render", &CDebugHelperImplementation::Render)
			.def("log", &CDebugHelperImplementation::Log)
			.def("register_bar", &CDebugHelperImplementation::RegisterBar)
			.def("remove_bar", &CDebugHelperImplementation::RemoveBar)
	];

	//ScriptManager
	module(m_LS)[
		class_<CScriptManager>("CScriptManager")
			.def(constructor<>())
			.def("runcode", &CScriptManager::RunCode)
	];


	// Engine-------------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CUABEngine>("CUABEngine")
			.def("get_input_manager", &CUABEngine::GetInputManager)
			//.def("get_static_mesh_manager", &CUABEngine::GetStaticMeshManager)
			//.def("get_layer_manager", &CUABEngine::GetLayerManager)
			.def("get_material_manager", &CUABEngine::GetMaterialManager)
			.def("get_texture_manager", &CUABEngine::GetTextureManager)
			.def("get_effect_manager", &CUABEngine::GetEffectManager)
			.def("get_render_manager", &CUABEngine::GetRenderManager)
			.def("get_animated_models_manager", &CUABEngine::GetAnimatedModelsManager)
			.def("get_script_manager", &CUABEngine::GetScriptManager)
			.def("get_camera_controller_manager", &CUABEngine::GetCameraControllerManager)
			.def("get_physX_manager", &CUABEngine::GetPhysXManager)
			.def("get_sound_manager", &CUABEngine::GetSoundManager)
			.def("get_video_manager", &CUABEngine::GetVideoManager)
			//.def("get_cinematic_manager", &CUABEngine::GetCinematicManager)
			.def("get_gui_manager", &CUABEngine::GetGUIManager)
			.def("get_level_manager", &CUABEngine::GetLevelManager)
			//.def("get_level_loaded", &CUABEngine::GetLevelLoaded)
			//.def("get_astar_manager", &CUABEngine::GetAStarManager)
			.scope[
				def("get_instance", &CUABEngine::GetInstance)
			]
			.def("destroy", &CUABEngine::Destroy)
			.def("init", &CUABEngine::Init)
			.def("is_paused", &CUABEngine::GetPause)
			.def("set_pause", &CUABEngine::SetPause)
			.def("get_time_scale", &CUABEngine::GetTimeScale)
			.def("set_time_scale", &CUABEngine::SetTimeScale)
			.def("quit", &CUABEngine::Quit)
			//.def("get_game_play_manager", &CUABEngine::GetGamePlayManager)
			.def("get_frustum_active", &CUABEngine::GetFrustumActive)
			.def("set_frustum_active", &CUABEngine::SetFrustumActive)
			.def("get_lua_reloaded", &CUABEngine::GetLuaReloaded)		
			.def("get_active_console", &CUABEngine::GetActiveConsole)
			.def("set_active_console", &CUABEngine::SetActiveConsole)
			.def("get_type_particle", &CUABEngine::GetTypeParticle)
			.def("reload_lua", &CUABEngine::ReloadLUA)
			.def("get_screen_size", &CUABEngine::GetScreenSize)
	];

	// InputManager-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CInputManager>("CInputManager")
			.def("get_action", &CInputManager::GetAction)
			.def("get_input", &CInputManager::GetInput)
			.def("is_action_active", &CInputManager::IsActionActive)
			.def("is_action_new", &CInputManager::IsActionNew)
			.def("is_action_released", &CInputManager::IsActionReleased)
			.def("was_action_active", &CInputManager::WasActionActive)
			.def("get_cursor", &CInputManager::GetCursor)
			.def("get_cursor_movement", &CInputManager::GetCursorMovement)
			.def("reload", &CInputManager::Reload)
	];
	

	module(m_LS)[
		class_<TLevelInfo>("TLevelInfo")
			.def_readwrite("id", &TLevelInfo::m_ID)
			.def_readwrite("loaded", &TLevelInfo::m_Loaded)
			.def_readwrite("directory", &TLevelInfo::m_Directory)
	];

	module(m_LS)[
		class_<CLevel, CNamed>("CLevel")
			.def(constructor<>())
			.def(constructor<std::string>())
			.def("load", &CLevel::Load)
			.def("unload", &CLevel::Unload)
			.def("reload", &CLevel::Reload)
			.def("get_static_mesh_manager", &CLevel::GetStaticMeshManager)
			.def("get_layer_manager", &CLevel::GetLayerManager)
			.def("get_material_manager", &CLevel::GetMaterialManager)
			.def("get_light_manager", &CLevel::GetLightManager)
			.def("get_cinematic_manager", &CLevel::GetCinematicManager)
			.def("get_particle_manager", &CLevel::GetParticleManager)
			.def("get_bilboard_manager", &CLevel::GetBilboardManager)
			.def("get_game_play_manager", &CLevel::GetGamePlayManager)
			.def("get_manchas_manager", &CLevel::GetManchasManager)
			.def("get_bilboard_manager", &CLevel::GetBilboardManager)
			.def("get_astar_manager", &CLevel::GetAStarManager)
			.def("set_game_play_manager", &CLevel::SetGamePlayManager)
			.def("set_has_to_update", &CLevel::SetHasToUpdate)
			.def("is_visible",&CLevel::IsVisible)
			.def("set_visible", &CLevel::SetVisible)
	];

	RegisterTemplatedVectorMapManager<CLevel>(m_LS);

	module(m_LS)[
		class_<CLevelManager, CTemplatedVectorMapManager<CLevel>>("CLevelManager")
			.def("update", &CLevelManager::Update)
			.def("get_level", &CLevelManager::GetResource)
			.def("get_resource_by_id", &CLevelManager::GetResourceById)
			.def("get_resource_map", &CLevelManager::GetResourcesMap)
			.def("get_resource_vector", &CLevelManager::GetResourcesVector)
			.def("get_level_info",&CLevelManager::GetLevelInfo)
			.def("load_file",&CLevelManager::LoadFile)
			.def("load_level", &CLevelManager::LoadLevel)
			.def("unload_level", &CLevelManager::UnloadLevel)
			.def("reload_level", &CLevelManager::ReloadLevel)
			.def("get_complete_layer", &CLevelManager::GetCompleteLayer)
			.def("reload_all_lua", &CLevelManager::ReloadAllLua)
			.def("load_file", &CLevelManager::LoadFile)
			.def("choose_scene_command_level",&CLevelManager::ChooseSceneRenderLevel)
			.def("execute_scene_commands",&CLevelManager::ExecuteSceneCommands)
			.def("get_actual_level", &CLevelManager::GetActualLevel)
			.def("set_actual_level", &CLevelManager::SetActualLevel)
			.def("get_scene_command_manager", &CLevelManager::GetSceneRendererCommandManager)
			.def("change_object_level", &CLevelManager::ChangeObjectLevel)
	];

// GRAPHICS-----------------------------------------------------------------------------------------
#ifdef _DEBUG
	module(m_LS)[
		class_<CDebugRender>("CDebugRender")
			.def(constructor<ID3D11Device*>())
			.def("get_simple_triangle", &CDebugRender::GetSimpleTriangle)
			.def("get_classic_blend_triangle", &CDebugRender::GetClassicBlendTriangle)
			.def("get_premult_blend_triangle", &CDebugRender::GetPremultBlendTriangle)
			.def("get_simple_cube", &CDebugRender::GetSimpleCube)
			.def("get_axis", &CDebugRender::GetAxis)
			.def("get_simple_triangle_bs_radi", &CDebugRender::GetSimpleTriangleBSRadi)
			.def("get_premult_blend_triangle_bs_radi", &CDebugRender::GetClassicBlendTriangleBSRadi)
			.def("get_premult_blend_triangle_bs_radi", &CDebugRender::GetPremultBlendTriangleBSRadi)
			.def("get_simple_cube_bs_radi", &CDebugRender::GetSimpleCubeBSRadi)
			.def("get_axis_bs_radi", &CDebugRender::GetAxisBSRadi)
			.def("get_simple_triangle_bb_min", &CDebugRender::GetSimpleTriangleBBMin)
			.def("get_classic_blend_triangle_bb_min", &CDebugRender::GetClassicBlendTriangleBBMin)
			.def("get_premult_blend_triangle_bb_min", &CDebugRender::GetPremultBlendTriangleBBMin)
			.def("get_simple_cube_bb_min", &CDebugRender::GetSimpleCubeBBMin)
			.def("get_axis_bb_min", &CDebugRender::GetAxisBBMin)
			.def("get_simple_triangle_bb_max", &CDebugRender::GetSimpleTriangleBBMax)
			.def("get_classic_blend_triangle_bb_max", &CDebugRender::GetClassicBlendTriangleBBMax)
			.def("get_premult_blend_triangle_bb_max", &CDebugRender::GetPremultBlendTriangleBBMax)
			.def("get_simple_cube_bb_max", &CDebugRender::GetSimpleCubeBBMax)
			.def("get_axis_bb_max", &CDebugRender::GetAxisBBMax)
	];
#endif

	// SceneRendererCommand

	module(m_LS)[
		class_<CSceneRendererCommand, bases<CActive,CNamed>>("CSceneRendererCommand")
			.def("execute",&CSceneRendererCommand::Execute)
	];

	RegisterTemplatedVectorMapManager<CSceneRendererCommand>(m_LS);
	module(m_LS)[
		class_<CSceneRendererCommandManager, CTemplatedVectorMapManager<CSceneRendererCommand>>("CSceneRendererCommandManager")
			.def(constructor<>())
			.def("reload",&CSceneRendererCommandManager::Reload)
	];

	// RenderableObjects------------------------------------------------------------------------------
	module(m_LS)[
		class_<CRenderableObject, bases<C3DElement, CNamed>>("CRenderableObject")
			.def("update", &CRenderableObject::Update)
			.def("render", &CRenderableObject::Render)
	];

	RegisterTemplatedVectorMapManager<CRenderableObject>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedVectorMapManager<CRenderableObject>>("CTemplatedVectorMapManager<CRenderableObject>")
			.def("get_resource", &CTemplatedMapManager<CRenderableObject>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CRenderableObject>::AddResource)
			.def("destroy", &CTemplatedMapManager<CRenderableObject>::Destroy)
	];*/

	RegisterTemplatedVectorMapManager<CRenderableObjectsManager>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedVectorMapManager<CRenderableObjectsManager>>("CTemplatedVectorMapManager<CRenderableObjectsManager>")
			.def("get_resource", &CTemplatedMapManager<CRenderableObjectsManager>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CRenderableObjectsManager>::AddResource)
			.def("destroy", &CTemplatedMapManager<CRenderableObjectsManager>::Destroy)
	];*/

	module(m_LS)[
		class_<CRenderableObjectsManager, CTemplatedVectorMapManager<CRenderableObject>>("CRenderableObjectsManager")
			.def(constructor<std::string>())
			.def("update", &CRenderableObjectsManager::Update)
			.def("render", &CRenderableObjectsManager::Render)
			.def("add_mesh_instance", (CRenderableObject*(CRenderableObjectsManager::*)(tinyxml2::XMLElement*, CLevel*, bool))&CRenderableObjectsManager::AddMeshInstance)
			.def("add_mesh_instance", (CRenderableObject*(CRenderableObjectsManager::*)(const std::string&, const std::string&, const Vect3f&, CLevel*, const Quatf, const float, const bool, bool))&CRenderableObjectsManager::AddMeshInstance)
			.def("add_animated_instance_model", (CRenderableObject*(CRenderableObjectsManager::*)(tinyxml2::XMLElement*, CLevel*, bool))&CRenderableObjectsManager::AddAnimatedInstanceModel)
			.def("add_animated_instance_model", (CRenderableObject*(CRenderableObjectsManager::*)(const std::string&, const std::string&, const Vect3f&, CLevel*, bool))&CRenderableObjectsManager::AddAnimatedInstanceModel)
			.def("get_resource", &CRenderableObjectsManager::GetResource)
			//.def("clean_up", &CRenderableObjectsManager::CleanUp)
			//.def("get_instance", &CRenderableObjectsManager::GetInstance)
	];

	// TODO A�adir LayerManager
	module(m_LS)[
		class_<CLayerManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>("CLayerManager")
			.def(constructor<>())
			.def("reload", &CLayerManager::Reload)
			.def("get_layer", (CRenderableObjectsManager*(CLayerManager::*)(std::string))&CLayerManager::GetLayer)
			.def("save", &CLayerManager::Save)
	];


	module(m_LS)[
		class_<CRenderableObjectTechnique>("CRenderableObjectTechnique")
			.def(constructor<std::string, CEffectTechnique*>())
			.property("effect_technique", &CRenderableObjectTechnique::GetEffectTechnique, &CRenderableObjectTechnique::SetEffectTechnique)
	];

	module(m_LS)[
		class_<CRenderableVertexs>("CRenderableVertexs")
			.def("render", &CRenderableVertexs::Render)
			.def("render_indexed", &CRenderableVertexs::RenderIndexed)
	];

	// AnimatedModels----------------------------------------------------------------------------------
	module(m_LS) [
		class_<CAnimatedCoreModel, CNamed>("CAnimatedCoreModel")
			.def(constructor<>())
			.def("get_cal_core_model", &CAnimatedCoreModel::GetCalCoreModel)
			.def("get_materials", &CAnimatedCoreModel::GetMaterials)
			.def("load",&CAnimatedCoreModel::Load)
	];

	RegisterTemplatedMapManager<CAnimatedCoreModel>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CAnimatedCoreModel>>("CTemplatedMapManager<CAnimatedCoreModel>")
			.def("get_resource", &CTemplatedMapManager<CAnimatedCoreModel>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CAnimatedCoreModel>::AddResource)
			.def("destroy", &CTemplatedMapManager<CAnimatedCoreModel>::Destroy)
	];*/

	module(m_LS) [
		class_<CAnimatedInstanceModel, CRenderableObject>("CAnimatedInstanceModel")
			.def(constructor<tinyxml2::XMLElement*,CLevel*>())
			.def("initialize", &CAnimatedInstanceModel::Initialize)
			.def("render", &CAnimatedInstanceModel::Render)
			.def("update", &CAnimatedInstanceModel::Update)
			.def("destroy", &CAnimatedInstanceModel::Destroy)
			.def("execute_action", &CAnimatedInstanceModel::ExecuteAction)
			.def("remove_action", &CAnimatedInstanceModel::RemoveAction)
			.def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
			.def("clear_cycle", &CAnimatedInstanceModel::ClearCycle)
			.def("remove_animations", &CAnimatedInstanceModel::RemoveAnimations)
			.def("is_cycle_animation_active", &CAnimatedInstanceModel::IsCycleAnimationActive)
			.def("is_action_animation_active", &CAnimatedInstanceModel::IsActionAnimationActive)
			.def("is_action_animation_ended", &CAnimatedInstanceModel::IsActionAnimationEnded)
			.def("get_actual_cycle_animation", &CAnimatedInstanceModel::GetActualCycleAnimation)
			.def("get_actual_action_animation", &CAnimatedInstanceModel::GetActualActionAnimation)
			.def("get_animation_time", &CAnimatedInstanceModel::GetAnimationTime)
			.def("get_animation_duration", &CAnimatedInstanceModel::GetAnimationDuration)
			.def("get_right_object_position", &CAnimatedInstanceModel::GetRightObjectPosition)
			.def("get_left_object_position", &CAnimatedInstanceModel::GetLeftObjectPosition)
			.def("get_right_object_rotation", &CAnimatedInstanceModel::GetRightObjectRotation)
			.def("get_left_object_rotation", &CAnimatedInstanceModel::GetLeftObjectRotation)
			.def("get_right_object_transform", &CAnimatedInstanceModel::GetRightObjectTransform)
			.def("get_left_object_transform", &CAnimatedInstanceModel::GetLeftObjectTransform)
			.def("get_bone_position", &CAnimatedInstanceModel::GetBonePosition)
			.def("get_bone_rotation", &CAnimatedInstanceModel::GetBoneRotation)
			.def("set_bone_rotation", &CAnimatedInstanceModel::SetBoneRotation)
			.def("print_bone_list", &CAnimatedInstanceModel::PrintBoneList)
	];

	module(m_LS) [
		class_<CAnimatedModelsManager, CTemplatedMapManager<CAnimatedCoreModel>>("CAnimatedModelsManager")
			.def(constructor<>())
			.def("load",&CAnimatedModelsManager::Load)
			.def("reload", &CAnimatedModelsManager::Reload)
	];	

	// Camera----------------------------------------------------------------------------------------
	module(m_LS) [
		class_<CCamera>("CCamera")
			.def(constructor<>())
			.def("set_fov", &CCamera::SetFOV)
			.def("get_fov", &CCamera::GetFOV)
			.def("set_aspect_ratio", &CCamera::SetAspectRatio)
			.def("get_aspect_ratio",&CCamera::GetAspectRatio)
			.def("set_z_near", &CCamera::GetZNear)
			.def("get_z_near", &CCamera::GetZNear)
			.def("set_z_far", &CCamera::SetZFar)
			.def("get_z_far", &CCamera::GetZFar)
			.def("set_position", &CCamera::SetPosition)
			.def("get_position", &CCamera::GetPosition)
			.def("set_look_at", &CCamera::SetLookAt)
			.def("get_look_at", &CCamera::GetLookAt)
			.def("set_up", &CCamera::SetUp)
			.def("get_up", &CCamera::GetUp)
			.def("get_view", &CCamera::GetView)
			.def("get_projection", &CCamera::GetProjection)
			.def("set_matrixs", &CCamera::SetMatrixs)
			.def("get_camera_type_by_name", &CCamera::GetCameraTypeByName)
	];

	module(m_LS)[
		class_<CCameraController,CNamed>("CCameraController")
			//.def(constructor<>())
			.def("set_camera", &CCameraController::SetCamera)
			.def("add_yaw", &CCameraController::AddYaw)
			.def("add_pitch", &CCameraController::AddPitch)
			.def("set_rotation", &CCameraController::SetRotation)
			.def("get_rotation", &CCameraController::GetRotation)
			.def("get_right", &CCameraController::GetRight)
			.def("get_up", &CCameraController::GetUp)
			.def("get_forward", &CCameraController::GetForward)
			.def("get_position", &CCameraController::GetPosition)
			.def("set_position", &CCameraController::SetPosition)
			.def("update", &CCameraController::Update)
			.def("set_fov", &CCameraController::SetFov)
			.def("get_fov", &CCameraController::GetFov)
	];

	RegisterTemplatedMapManager<CCameraController>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CCameraController>>("CTemplatedMapManager<CCameraController>")
			.def("get_resource", &CTemplatedMapManager<CCameraController>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CCameraController>::AddResource)
			.def("destroy", &CTemplatedMapManager<CCameraController>::Destroy)
	];*/

	module(m_LS)[
		class_<CCameraControllerManager, CTemplatedMapManager<CCameraController>>("CCameraControllerManager")
			.def(constructor<>())
			.def("choose_main_camera", &CCameraControllerManager::ChooseMainCamera)
			.def("choose_debug_camera", &CCameraControllerManager::ChooseDebugCamera)
			.def("load", &CCameraControllerManager::Load)
			.def("reload", &CCameraControllerManager::Reload)
			.def("get_main_camera", &CCameraControllerManager::GetMainCamera)
			.def("get_debug_camera", &CCameraControllerManager::GetDebugCamera)
			.def("update", &CCameraControllerManager::Update)
			.def("init", &CCameraControllerManager::Init)
	];

	module(m_LS)[
		class_<CCameraInfo>("CCameraInfo")
			.def(constructor<>())
			.def(constructor<const Vect3f, const Vect3f, const Vect3f, /*float, float,*/ float>())
			.def(constructor<tinyxml2::XMLElement*>())
			//.def("set_near_plane", &CCameraInfo::SetNearPlane)
			//.def("get_near_plane", &CCameraInfo::GetNearPlane)
			//.def("set_far_plane", &CCameraInfo::SetFarPlane)
			//.def("get_far_plane", &CCameraInfo::GetFarPlane)
			.def("set_fov", &CCameraInfo::SetFOV)
			.def("get_fov", &CCameraInfo::GetFOV)
			.def("set_eye", &CCameraInfo::SetEye)
			.def("get_eye", &CCameraInfo::GetEye)
			.def("set_look_at", &CCameraInfo::SetLookAt)
			.def("get_look_at", &CCameraInfo::GetLookAt)
			.def("set_up", &CCameraInfo::SetUp)
			.def("get_up", &CCameraInfo::GetUp)
	];

	module(m_LS)[
		class_<CCameraKey>("CCameraKey")
			.def(constructor<CCameraInfo&, float>())
			.def("set_camera_info", &CCameraKey::SetCameraInfo)
			.def("get_camera_info", &CCameraKey::GetCameraInfo)
	];

	module(m_LS)[
		class_<CCameraKeyController, CCameraController>("CCameraKeyController")
			.def(constructor<tinyxml2::XMLElement*, const std::string &>())
			.def("update", &CCameraKeyController::Update)
			.def("set_current_time", &CCameraKeyController::SetCurrentTime)
			.def("reset_time", &CCameraKeyController::ResetTime)
			.def("get_total_time", &CCameraKeyController::GetTotalTime)
			.def("is_cycle", &CCameraKeyController::IsCycle)
			.def("set_cycle", &CCameraKeyController::SetCycle)
			.def("is_reverse", &CCameraKeyController::IsReverse)
			.def("set_reverse", &CCameraKeyController::SetReverse)
			.def("set_camera", &CCameraKeyController::SetCamera)
			.def("set_first_key", &CCameraKeyController::SetFirstKey)
			.def("get_camera_as_info", &CCameraKeyController::GetCameraAsInfo)
			.def("get_camera_key", &CCameraKeyController::GetCameraKey)
			.def("force_update_yaw", &CCameraKeyController::ForceUpdateYaw)
			.def("set_look_at", &CCameraKeyController::SetLookAt)
			.def("set_up", &CCameraKeyController::SetUp)
			.def_readwrite("m_PositionOffsetKey", &CCameraKeyController::m_PositionOffsetKey)
			.def_readwrite("m_PositionOffset", &CCameraKeyController::m_PositionOffset)
			.def_readwrite("m_RotationOffset", &CCameraKeyController::m_RotationOffset)
	];

	module(m_LS)[
		class_<C3PersonCameraController, CCameraController>("C3PersonCameraController")
			.def(constructor<tinyxml2::XMLElement*, const std::string &>())
			.def("get_direction",&C3PersonCameraController::GetDirection)
	];

	module(m_LS) [
		class_<CFPSCameraController, CCameraController>("CFPSCameraController")
			.def(constructor<tinyxml2::XMLElement*, const std::string &>())
			//.def("move", &CFPSCameraController::Move)
			.def("set_camera", &CFPSCameraController::SetCamera)
			.def("add_yaw", &CFPSCameraController::AddYaw)
			.def("add_pitch",&CFPSCameraController::AddPitch)
			.def("get_forward", &CFPSCameraController::GetForward)
			.def("lock", &CFPSCameraController::Lock)
			.def("unlock", &CFPSCameraController::Unlock)
			.def("copy_from_key_camera", &CFPSCameraController::CopyFromKeyCamera)
			.def("copy_from_camera", &CFPSCameraController::CopyFromCamera)
			.def("update_position_from_target", &CFPSCameraController::UpdatePositionFromTarget)
	];

	module(m_LS) [
		class_<CFrustum>("CFrustum")
			.def("update", &CFrustum::Update)
			.def("sphere_visible", &CFrustum::SphereVisible)
			.def("box_visible",&CFrustum::BoxVisible)
	];

	module(m_LS) [
		class_<CSphericalCameraController, CCameraController>("CSphericalCameraController")
			.def(constructor<tinyxml2::XMLElement*, const std::string &>())
			.def("add_zoom", &CSphericalCameraController::AddZoom)
			.def("set_zoom", &CSphericalCameraController::SetZoom)
			.def("set_camera", &CSphericalCameraController::SetCamera)
			.def("rotate", &CSphericalCameraController::Rotate)
	];

	module(m_LS)[
		class_<CFocusedCameraController, CCameraController>("CFocusedCameraController")
			.def(constructor<tinyxml2::XMLElement*, const std::string &>())
			.def("init", &CFocusedCameraController::Init)
			.def("set_focus", &CFocusedCameraController::SetFocus)
			.def("set_start", &CFocusedCameraController::SetStart)
			.def("set_camera", &CFocusedCameraController::SetCamera)
			.def("get_lookat", &CFocusedCameraController::GetLookAt)
			.def("copy_to_camera", &CFocusedCameraController::CopyToCamera)
	];

	// Cinematics -----------------------------------------------------------------------------------
	
	module(m_LS)[
		class_<CCinematicPlayer>("CCinematicPlayer")
			.def(constructor<>())
			.def("init", &CCinematicPlayer::Init)
			.def("update", &CCinematicPlayer::Update)
			.def("stop", &CCinematicPlayer::Stop)
			.def("play", &CCinematicPlayer::Play)
			.def("pause", &CCinematicPlayer::Pause)
			.def("get_duration", &CCinematicPlayer::GetDuration)
			.def("get_current_time", &CCinematicPlayer::GetCurrentT)
			.def("on_restart_cycle", &CCinematicPlayer::OnRestartCycle)
	];

	module(m_LS)[
		class_<CCinematic, bases<CNamed, CCinematicPlayer>>("CCinematic")
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("add_cinematic_object", &CCinematic::AddCinematicObject)
			.def("update", &CCinematic::Update)
			.def("play",&CCinematic::Play)
			.def("stop",&CCinematic::Stop)
			.def("pause", &CCinematic::Pause)
			.def("on_restart_cycle", &CCinematic::OnRestartCycle)
			.def("is_finished", &CCinematic::IsFinished)
			.def("get_duration", &CCinematic::GetDuration)
	];

	module(m_LS)[
		class_<CCinematicObject, CCinematicPlayer>("CCinematicObject")
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("is_ok", &CCinematicObject::IsOk)
			.def("add_cinematic_object_key_frame", &CCinematicObject::AddCinematicObjectKeyFrame)
			.def("update", &CCinematicObject::Update)
			.def("on_restart_cycle", &CCinematicObject::OnRestartCycle)
			.def("get_current_key", &CCinematicObject::GetCurrentKey)
			.def("is_finished", &CCinematicObject::IsFinished)
	];

	module(m_LS)[
		class_<CCinematicObjectKeyFrame, C3DElement>("CCinematicObjectKeyFrame")
			.def(constructor<tinyxml2::XMLElement*>())
			.def("get_key_frame_time",&CCinematicObjectKeyFrame::GetKeyFrameTime)
			.def("set_key_frame_time",&CCinematicObjectKeyFrame::SetKeyFrameTime)
	];

	RegisterTemplatedMapManager<CCinematic>(m_LS);

	module(m_LS)[
		class_<CCinematicManager, bases<CRenderableObject, CTemplatedMapManager<CCinematic>>>("CCinematicManager")
			.def(constructor<CLevel*>())
			.def("load_xml", &CCinematicManager::LoadXML)
			.def("reload", &CCinematicManager::Reload)
			.def("update", &CCinematicManager::Update)
			.def("render", &CCinematicManager::Render)
	];

	// ContextManager----------------------------------------------------------------------------------
	module(m_LS)[
		class_<CContextManager>("CContextManager")
			.def(constructor<>())
			.def("dispose", &CContextManager::Dispose)
			.def("resize", &CContextManager::Resize)
			.def("create_context", &CContextManager::CreateContext)
			.def("create_back_buffer", &CContextManager::CreateBackBuffer)
			.def("init_states", &CContextManager::InitStates)
			.def("get_aspect_ratio", &CContextManager::GetAspectRatio)
			.def("begin_render", &CContextManager::BeginRender)
			.def("end_render", &CContextManager::EndRender)
			//.def("draw", &CContextManager::Draw)
			.def("get_device", &CContextManager::GetDevice)
			.def("get_device_context", &CContextManager::GetDeviceContext)
			//.def("set_base_color", &CContextManager::SetBaseColor)
			.def("set_world_matrix", &CContextManager::SetWorldMatrix)
			.def("set_camera", (void(CContextManager::*)(const Mat44f&, const Mat44f&))&CContextManager::SetCamera)
			.def("set_camera", (void(CContextManager::*)(const CCamera&))&CContextManager::SetCamera)
			//.def("set_debug_size", &CContextManager::SetDebugSize)
	];

	// Effects----------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CAnimatedModelEffectParameters>("CAnimatedModelEffectParameters")
	];


	module(m_LS)[
		class_<CEffectShader, CNamed>("CEffectShader")
			.def("load", &CEffectShader::Load)
			.def("reload", &CEffectShader::Reload)
			.def("set_constant_buffer", &CEffectShader::SetConstantBuffer)
			.def("get_constant_buffer", &CEffectShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectVertexShader, CEffectShader>("CEffectVertexShader")
			.def(constructor<tinyxml2::XMLElement*>())
			.def("load", &CEffectVertexShader::Load)
			.def("set_constant_buffer", &CEffectVertexShader::SetConstantBuffer)
			.def("reload", &CEffectVertexShader::Reload)
			.def("get_vertex_shader", &CEffectVertexShader::GetVertexShader)
			.def("get_vertex_layout", &CEffectVertexShader::GetVertexLayout)
			//.def("get_constant_buffer", &CEffectVertexShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectPixelShader, CEffectShader>("CEffectPixelShader")
			.def(constructor<tinyxml2::XMLElement*>())
			.def("reload", &CEffectPixelShader::Reload)
			.def("load", &CEffectPixelShader::Load)
			.def("set_constant_buffer", &CEffectPixelShader::SetConstantBuffer)
			.def("get_pixel_shader", &CEffectPixelShader::GetPixelShader)
			//.def("get_constant_buffer", &CEffectPixelShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectGeometryShader, CEffectShader>("CEffectGeometryShader")
			.def(constructor<tinyxml2::XMLElement*>())
			.def("reload", &CEffectGeometryShader::Reload)
			.def("load", &CEffectGeometryShader::Load)
			.def("set_constant_buffer", &CEffectGeometryShader::SetConstantBuffer)
			.def("get_geometry_shader", &CEffectGeometryShader::GetGeometryShader)
			//.def("get_constant_buffer", &CEffectPixelShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectTechnique, CNamed>("CEffectTechnique")
			.def(constructor<tinyxml2::XMLElement*>())
			.def("get_vertex_shader", &CEffectTechnique::GetVertexShader)
			.def("get_pixel_shader", &CEffectTechnique::GetPixelShader)
			.def("get_geometry_shader", &CEffectTechnique::GetGeometryShader)
			.def("refresh", &CEffectTechnique::Refresh)
			.def("set_constant_buffer", &CEffectTechnique::SetConstantBuffer)
	];

	RegisterTemplatedMapManager<CEffectTechnique>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CEffectTechnique>>("CTemplatedMapManager<CEffectTechnique>")
			.def("get_resource", &CTemplatedMapManager<CEffectTechnique>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CEffectTechnique>::AddResource)
			.def("destroy", &CTemplatedMapManager<CEffectTechnique>::Destroy)
			.def("size", &CTemplatedMapManager<CEffectTechnique>::Size)
			.def("get_map", &CTemplatedMapManager<CEffectTechnique>::GetResourcesMap)
			.def("get_elements_array", &CTemplatedMapManager<CEffectTechnique>::GetElementsArray)
	];*/

	module(m_LS)[
		class_<CEffectManager, CTemplatedMapManager<CEffectTechnique>>("CEffectManager")
			.def(constructor<>())
			.def("reload_file", &CEffectManager::ReloadFile)
			.def("reload", &CEffectManager::Reload)
			.def("load", &CEffectManager::Load)
			.def("get_vertex_shader", &CEffectManager::GetVertexShader)
			.def("get_pixel_shader", &CEffectManager::GetPixelShader)
			.def("get_geometry_shader", &CEffectManager::GetGeometryShader)
			.def("set_scene_constants", &CEffectManager::SetSceneConstants)
			.def("set_light_constants", &CEffectManager::SetLightConstants)
			.def("set_lights_constants", &CEffectManager::SetLightsConstants)
			.def("get_effects_names", &CEffectManager::GetEffectsNames)
	];

	module(m_LS)[
		class_<CLightEffectParameters>("CLightEffectParameters")
	];

	module(m_LS)[
		class_<CSceneEffectParameters>("CSceneEffectParameters")
	];

	// Lights-----------------------------------------------------------------------------------------

	module(m_LS)[
		class_<CLight, CNamed>("CLight")
			.def("get_position", &CLight::GetPosition)
			.def("set_position", &CLight::SetPosition)
			.def("get_color", &CLight::GetColor)
			.def("get_color_lua_address", &CLight::GetColorLuaAdress)
			.def("set_color", &CLight::SetColor)
			.def("get_intensity", &CLight::GetIntensity)
			.def("set_intensity", &CLight::SetIntensity)
			.def("get_start_range_attenuation", &CLight::GetStartRangeAttenuation)
			.def("set_start_range_attenuation", &CLight::SetStartRangeAttenuation)
			.def("get_end_range_attenuation", &CLight::GetEndRangeAttenuation)
			.def("set_end_range_attenuation", &CLight::SetEndRangeAttenuation)
			.def("get_enabled", &CLight::GetEnabled)
			.def("set_enabled", &CLight::SetEnabled)
			.def("get_type", &CLight::GetType)
			.def("set_type", &CLight::SetType)
			.def("render", &CLight::Render)
			.def("get_light_type_by_name", &CLight::GetLightTypeByName)
			.def("get_generate_shadowmap_lua_address", &CLight::GetGenerateShadowMapLuaAdress)
			.def("get_enable_lua_address", &CLight::GetEnableLuaAdress)
			.def("get_intensity_lua_address", &CLight::GetIntensityLuaAdress)
			.def("get_position_lua_address", &CLight::GetPositionLuaAddress)
			.def("get_start_range_attenuation_lua_address", &CLight::GetStartRangeAttenuationLuaAdress)
			.def("get_end_range_attenuation_lua_address", &CLight::GetEndRangeAttenuattionLuaAdress)
	];

	RegisterTemplatedVectorMapManager<CLight>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedVectorMapManager<CLight>>("CTemplatedVectorMapManager<CLight>")
			.def("get_resource", &CTemplatedMapManager<CLight>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CLight>::AddResource)
			.def("destroy", &CTemplatedMapManager<CLight>::Destroy)
	];*/

	module(m_LS)[
		class_<CLightManager, CTemplatedVectorMapManager<CLight>>("CLightManager")
			.def(constructor<>())
			.def("load", &CLightManager::Load)
#ifdef _DEBUG
			.def("render", &CLightManager::Render)
#endif
			.def("reload", &CLightManager::Reload)
			.def("get_ambient_light", &CLightManager::GetAmbientLight)
			.def("save", &CLightManager::Save)
			.def("create_new_light", &CLightManager::CreateNewLight)
	];

	module(m_LS)[
		class_<COmniLight,CLight>("COmniLight")
			.def(constructor<CLevel*>())
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
	];

	module(m_LS)[
		class_<CDirectionalLight, CLight>("CDirectionalLight")
			.def(constructor<CLevel*>())
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("get_direction", &CDirectionalLight::GetDirection)
			.def("set_direction", &CDirectionalLight::SetDirection)
			.def("get_direction_lua_address", &CDirectionalLight::GetDirectionLuaAdress)
			.def("render", &CDirectionalLight::Render)
	];

	module(m_LS)[
		class_<CSpotLight, CDirectionalLight>("CSpotLight")
			.def(constructor<CLevel*>())
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("get_angle", &CSpotLight::GetAngle)
			.def("set_angle", &CSpotLight::SetAngle)
			.def("get_fall_off", &CSpotLight::GetFallOff)
			.def("set_fall_off", &CSpotLight::SetFallOff)
			.def("get_angle_lua_address",&CSpotLight::GetAngleLuaAdress)
			.def("get_falloff_lua_address",&CSpotLight::GetFallOffLuaAdress)
	];

	// Materials--------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CMaterialParameter, CNamed>("CMaterialParameter")
			.enum_("t_material_type")[
				value("float",CMaterialParameter::FLOAT),
				value("vect2f", CMaterialParameter::VECT2F),
				value("vect3f", CMaterialParameter::VECT3F),
				value("vect4f", CMaterialParameter::VECT4F),
				value("color", CMaterialParameter::COLOR)
			]
			.def("apply", &CMaterialParameter::Apply)
			.def("get_material_type", &CMaterialParameter::getMaterialType)
			.def("get_value_address", &CMaterialParameter::GetValueLuaAddress)			
			.def("get_description", &CMaterialParameter::GetDescription)
			.scope[
				def("get_type_from_string",&CMaterialParameter::GetTypeFromString)
			]
	];

	module(m_LS)[
		class_<CMaterial, CNamed>("CMaterial")
			.def(constructor<tinyxml2::XMLElement*,const std::string &>())
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("apply", &CMaterial::Apply)
			.def("get_next_parameter_adress", &CMaterial::GetNextParameterAddress)
			.def("get_parameters", &CMaterial::GetParameters, luabind::return_stl_iterator)
			.def("get_renderable_object_technique", &CMaterial::GetRenderableObjectTechnique)
			.def("get_texture", &CMaterial::GetTexture)
			.def("set_value", (void(CMaterial::*)(const int, const float))&CMaterial::SetValue)
			.def("set_value", (void(CMaterial::*)(const int, const CColor))&CMaterial::SetValue)
			.def("get_value", &CMaterial::GetValue)
	];

	RegisterTemplatedMapManager<CMaterial>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CMaterial>>("CTemplatedMapManager<CMaterial>")
			.def("get_resource", &CTemplatedMapManager<CMaterial>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CMaterial>::AddResource)
			.def("destroy", &CTemplatedMapManager<CMaterial>::Destroy)
	];*/

	module(m_LS)[
		class_<CMaterialManager, CTemplatedMapManager<CMaterial>>("CMaterialManager")
			.def(constructor<>())
			.def("load", &CMaterialManager::Load)
			.def("reload", &CMaterialManager::Reload)
			.def("save", &CMaterialManager::Save)
	];


	// RenderManager----------------------------------------------------------------------------------
	module(m_LS)[
		class_<CRenderManager>("CRenderManager")
			.def(constructor<>())
			.def("set_current_camera", &CRenderManager::SetCurrentCamera)
			.def("set_debug_camera", &CRenderManager::SetDebugCamera)
			.def("set_use_debug_camera", &CRenderManager::SetUseDebugCamera)
			//.def("add_renderable_object_to_render_list", &CRenderManager::AddRenderableObjectToRenderList)
			.def("render", &CRenderManager::Render)
			.def("get_frame_rate", &CRenderManager::GetFrameRate)
			.def("get_frame_rate_address", &CRenderManager::GetFrameRateAddress)
	];

	// StaticMesh-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CInstanceMesh, CRenderableObject>("CInstanceMesh")
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def(constructor<const std::string&, const std::string&, CLevel*>())
			.def("render", &CInstanceMesh::Render)
			.def("get_interactuable_object_name", &CInstanceMesh::GetInteractuableObject)
			.def("change_level",&CInstanceMesh::ChangeLevel)
	];

	module(m_LS)[
		class_<CStaticMesh, CNamed>("CStaticMesh")
			.def(constructor<CLevel*>())
			.def("load", &CStaticMesh::Load)
			.def("reload", &CStaticMesh::Reload)
			.def("render", &CStaticMesh::Render)
	];

	RegisterTemplatedMapManager<CStaticMesh>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CStaticMesh>>("CTemplatedMapManager<CStaticMesh>")
			.def("get_resource", &CTemplatedMapManager<CStaticMesh>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CStaticMesh>::AddResource)
			.def("destroy", &CTemplatedMapManager<CStaticMesh>::Destroy)
	];*/

	module(m_LS)[
		class_<CStaticMeshManager, CTemplatedMapManager<CStaticMesh>>("CStaticMeshManager")
			.def(constructor<>())
			.def("load", &CStaticMeshManager::Load)
			.def("reload", &CStaticMeshManager::Reload)
	];

	// Texture----------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CTexture, CNamed>("CTexture")
			.def(constructor<>())
			.def("load", &CTexture::Load)
			.def("activate", &CTexture::Activate)
			.def("reload", &CTexture::Reload)
	];

	RegisterTemplatedMapManager<CTexture>(m_LS);
	/*module(m_LS)[
		class_<CTemplatedMapManager<CTexture>>("CTemplatedMapManager<CTexture>")
			.def("get_resource", &CTemplatedMapManager<CTexture>::GetResource)
			.def("add_resource", &CTemplatedMapManager<CTexture>::AddResource)
			.def("destroy", &CTemplatedMapManager<CTexture>::Destroy)
	];*/

	module(m_LS)[
		class_<CTextureManager>("CTextureManager")
			.def(constructor<>())
			.def("get_texture", &CTextureManager::GetTexture)
			.def("add_texture", &CTextureManager::AddTexture)
			.def("add_update_texture", &CTextureManager::AddUpdateTexture)
			.def("remove_texture", &CTextureManager::RemoveTexture)
			.def("reload", &CTextureManager::Reload)
	];


	// Particles-----------------------------------------------------------------------------------------

	module(m_LS)[
	class_<CParticleSystemType, CNamed>("CParticleSystemType")
		.def("get_angular_acceleration", &CParticleSystemType::GetAngularAcceleration)
		.def("set_angular_acceleration", &CParticleSystemType::SetAngularAcceleration)
		.def("get_awake_time", &CParticleSystemType::GetAwakeTime)
		.def("set_awake_time", &CParticleSystemType::SetAwakeTime)
		.def("get_emit_absolute", &CParticleSystemType::GetEmitAbsolute)
		.def("set_emit_absolute", &CParticleSystemType::SetEmitAbsolute)
		.def("get_emit_rate", &CParticleSystemType::GetEmitRate)
		.def("set_emit_rate", &CParticleSystemType::SetEmitRate)
		.def("get_life", &CParticleSystemType::GetLife)
		.def("set_life", &CParticleSystemType::SetLife)
		.def("get_loop_frames", &CParticleSystemType::GetLoopFrames)
		.def("set_loop_frames", &CParticleSystemType::SetLoopFrames)
		.def("get_name", &CParticleSystemType::GetName)
		.def("set_name", &CParticleSystemType::SetName)
		.def("get_num_frames", &CParticleSystemType::GetNumFrames)
		.def("set_num_frames", &CParticleSystemType::SetNumFrames)
		.def("get_sleep_time", &CParticleSystemType::GetSleepTime)
		.def("set_sleep_time", &CParticleSystemType::SetSleepTime)
		.def("get_starting_acceleration_angle", &CParticleSystemType::GetStartingAccelerationAngle)
		.def("set_starting_acceleration_angle", &CParticleSystemType::SetStartingAccelerationAngle)
		.def("get_starting_angle", &CParticleSystemType::GetStartingAngle)
		.def("set_starting_angle", &CParticleSystemType::SetStartingAngle)
		.def("get_starting_angular_speed", &CParticleSystemType::GetStartingAngularSpeed)
		.def("set_starting_angular_speed", &CParticleSystemType::SetStartingAngularSpeed)
		.def("get_starting_direction_angle", &CParticleSystemType::GetStartingDirectionAngle)
		.def("set_starting_direction_angle", &CParticleSystemType::SetStartingDirectionAngle)
		.def("get_time_per_frame", &CParticleSystemType::GetTimePerFrame)
		.def("set_time_per_frame", &CParticleSystemType::SetTimePerFrame)
		.def("get_material", &CParticleSystemType::GetMaterial)
		.def("set_material", &CParticleSystemType::SetMaterial)
		.def("get_sizes_control_points_size", &CParticleSystemType::GetSizesControlPointSize)
		.def("get_colors_control_points_size", &CParticleSystemType::GetColorsControlPointSize)
		.def("get_speeds_control_points_size", &CParticleSystemType::GetSpeedsControlPointSize)
		.def("get_accelerations_control_points_size", &CParticleSystemType::GetAccelerationsControlPointSize)
		.def("get_lua_angular_acceleration",  &CParticleSystemType::GetLuaAngularAcceleration)
		.def("get_lua_awake_time", &CParticleSystemType::GetLuaAwakeTime)
		.def("get_emit_absolute", &CParticleSystemType::GetLuaEmitAbsolute)
		.def("get_emit_rate", &CParticleSystemType::GetLuaEmitRate)
		.def("get_lua_loop_frames", &CParticleSystemType::GetLuaLoopFrames)
		.def("get_lua_name_address", &CParticleSystemType::GetLuaNameAddress)
		.def("get_lua_num_frames", &CParticleSystemType::GetLuaNumFrames)
		.def("get_lua_sleep_time", &CParticleSystemType::GetLuaSleepTime)
		.def("get_lua_starting_acceleration_angle", &CParticleSystemType::GetLuaStartingAccelerationAngle)
		.def("get_lua_starting_angle", &CParticleSystemType::GetLuaStartingAngle)
		.def("get_lua_starting_angular_speed", &CParticleSystemType::GetLuaStartingAngularSpeed)
		.def("get_lua_starting_direction_angle", &CParticleSystemType::GetLuaStartingDirectionAngle)
		.def("get_lua_time_per_frame", &CParticleSystemType::GetLuaTimePerFrame)
		.def("get_lua_life", &CParticleSystemType::GetLuaLife)
		.def("get_lua_control_point_size", &CParticleSystemType::GetLuaCPSize)
		.def("get_lua_control_point_size_time", &CParticleSystemType::GetLuaCPSizeTime)
		.def("get_lua_control_point_color1", &CParticleSystemType::GetLuaCPColor1)
		.def("get_lua_control_point_color2", &CParticleSystemType::GetLuaCPColor2)
		.def("get_lua_control_point_color_time", &CParticleSystemType::GetLuaCPColorTime)
		.def("get_lua_control_point_speed1", &CParticleSystemType::GetLuaCPSpeed1)
		.def("get_lua_control_point_speed2", &CParticleSystemType::GetLuaCPSpeed2)
		.def("get_lua_control_point_speed_time", &CParticleSystemType::GetLuaCPSpeedTime)
		.def("get_lua_control_point_acceleration1", &CParticleSystemType::GetLuaCPAcceleration1)
		.def("get_lua_control_point_acceleration2", &CParticleSystemType::GetLuaCPAcceleration2)
		.def("get_lua_control_point_acceleration_time", &CParticleSystemType::GetLuaCPAccelerationTime)
	];

	RegisterTemplatedMapManager<CParticleSystemType>(m_LS);

	module(m_LS)[
	class_<CParticleManager, CTemplatedMapManager<CParticleSystemType>>("CParticleManager")
		.def(constructor<>())
		.def("load", &CParticleManager::Load)
		.def("reload", &CParticleManager::Reload)
		.def("save", &CParticleManager::Save)
	];

	module(m_LS)[
	class_<CParticleSystemInstance, CRenderableObject>("CParticleSystemInstance")
		.def(constructor<CLevel*>())
		.def(constructor<tinyxml2::XMLElement*, CLevel*>())
		.def("set_active_particles", &CParticleSystemInstance::SetActiveParticles)
		.def("get_active_particles", &CParticleSystemInstance::GetActiveParticles)
		.def("get_start", &CParticleSystemInstance::GetStart)
		.def("set_start", &CParticleSystemInstance::SetStart)
		.def("get_awake", &CParticleSystemInstance::GetAwake)
		.def("set_awake", &CParticleSystemInstance::SetAwake)
		.def("get_awake_timer", &CParticleSystemInstance::GetAwakeTimer)
		.def("set_awake_timer", &CParticleSystemInstance::SetAwakeTimer)
		.def("get_emission_box_limit", &CParticleSystemInstance::GetEmissionBoxLimit)
		.def("set_emission_box_limit", &CParticleSystemInstance::SetEmissionBoxLimit)
		.def("get_emission_box_half_size", &CParticleSystemInstance::GetEmissionBoxHalfSize)
		.def("set_emission_box_half_size", &CParticleSystemInstance::SetEmissionBoxHalfSize)
		.def("get_emission_scaler", &CParticleSystemInstance::GetEmissionScaler)
		.def("set_emission_scaler", &CParticleSystemInstance::SetEmissionScaler)
		.def("get_emission_volume", &CParticleSystemInstance::GetEmissionVolume)
		.def("set_emission_volume", &CParticleSystemInstance::SetEmissionVolume)
		.def("get_name", &CParticleSystemInstance::GetName)
		.def("set_name", &CParticleSystemInstance::SetName)
		.def("get_position", &CParticleSystemInstance::GetPosition)
		.def("set_position", &CParticleSystemInstance::SetPosition)
		.def("get_rotation", &CParticleSystemInstance::GetRotation)
		.def("set_rotation", &CParticleSystemInstance::SetRotation)
		.def("get_scale", &CParticleSystemInstance::GetScale)
		.def("set_scale", &CParticleSystemInstance::SetScale)
		.def("get_type", &CParticleSystemInstance::GetType)
		.def("set_type", &CParticleSystemInstance::SetType)
		.def("get_visible", &CParticleSystemInstance::GetVisible)
		.def("set_visible", &CParticleSystemInstance::SetVisible)
		.def("insert_sort", &CParticleSystemInstance::InsertSort)
		.def("get_lua_emission_box_limit", &CParticleSystemInstance::GetLuaEmissionBoxLimit)
		.def("get_lua_emission_box_position", &CParticleSystemInstance::GetLuaEmissionBoxPosition)
		.def("get_lua_emission_box_half_size", &CParticleSystemInstance::GetLuaEmissionBoxHalfSize)
	];

	module(m_LS)[
		class_<CBilboardSystemType, CNamed>("CBilboardSystemType")
		.def("get_lua_num_frames", &CBilboardSystemType::GetLuaNumFrames)
		.def("get_lua_time_per_frame", &CBilboardSystemType::GetLuaTimePerFrame)
		.def("get_material", &CBilboardSystemType::GetMaterial)
	];

	RegisterTemplatedMapManager<CBilboardSystemType>(m_LS);

	module(m_LS)[
		class_<CBilboardManager, CTemplatedMapManager<CBilboardSystemType>>("CBilboardManager")
		.def(constructor<>())
		.def("load", &CBilboardManager::Load)
		.def("reload", &CBilboardManager::Reload)
	];

	module(m_LS)[
		class_<CBilboardSystemInstance, CRenderableObject>("CBilboardSystemInstance")
		.def(constructor<CLevel*>())
		.def(constructor<tinyxml2::XMLElement*, CLevel*>())
		.def("get_lua_sizeX", &CBilboardSystemInstance::GetLuaSizeX)
		.def("get_lua_sizeY", &CBilboardSystemInstance::GetLuaSizeY)
		.def("get_lua_size_offset", &CBilboardSystemInstance::GetLuaSizeOffset)
		.def("get_lua_color", &CBilboardSystemInstance::GetLuaColor)
		.def("get_type", &CBilboardSystemInstance::GetType)
		.def("get_active_bilboards", &CBilboardSystemInstance::GetActiveBilboards)
		.def("get_position_lua_address", &CBilboardSystemInstance::GetLuaPosition)
	];

	// Manchas-----------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CManchasSystemType, CNamed>("CManchasSystemType")
		.def("get_frames", &CManchasSystemType::GetFrames)
		.def("set_frames", &CManchasSystemType::SetFrames)
		.def("get_emit_time", &CManchasSystemType::GetEmitTime)
		.def("set_emit_time", &CManchasSystemType::SetEmitTime)
		.def("get_life", &CManchasSystemType::GetLife)
		.def("set_life", &CManchasSystemType::SetLife)
		.def("get_opacity", &CManchasSystemType::GetOpacity)
		.def("set_opacity", &CManchasSystemType::SetOpacity)
		.def("get_size", &CManchasSystemType::GetSize)
		.def("set_size", &CManchasSystemType::SetSize)
		.def("get_size_speed", &CManchasSystemType::GetSizeSpeed)
		.def("set_size_speed", &CManchasSystemType::SetSizeSpeed)
		.def("get_color1", &CManchasSystemType::GetColor1)
		.def("set_color1", &CManchasSystemType::SetColor1)
		.def("get_color2", &CManchasSystemType::GetColor2)
		.def("set_color2", &CManchasSystemType::SetColor2)
		.def("get_lua_frames", &CManchasSystemType::GetLuaFrames)
		.def("get_lua_emit_time", &CManchasSystemType::GetLuaEmitTime)
		.def("get_lua_life_time", &CManchasSystemType::GetLuaLife)
		.def("get_lua_opacity", &CManchasSystemType::GetLuaOpacity)
		.def("get_lua_size", &CManchasSystemType::GetLuaSize)
		.def("get_lua_size_speed", &CManchasSystemType::GetLuaSizeSpeed)
		.def("get_lua_color1", &CManchasSystemType::GetLuaColor1)
		.def("get_lua_color2", &CManchasSystemType::GetLuaColor2)
	];

	RegisterTemplatedMapManager<CManchasSystemType>(m_LS);

	module(m_LS)[
		class_<CManchasManager, CTemplatedMapManager<CManchasSystemType>>("CManchasManager")
			.def(constructor<>())
			.def("load", &CManchasManager::Load)
			.def("reload", &CManchasManager::Reload)
			.def("save", &CManchasManager::Save)
	];

	module(m_LS)[
		class_<CManchasSystemInstance, CRenderableObject>("CManchasSystemInstance")
			.def(constructor<tinyxml2::XMLElement*, CLevel*>())
			.def("get_type", &CManchasSystemInstance::GetType)
			.def("get_awake", &CManchasSystemInstance::GetAwake)
			.def("set_awake", &CManchasSystemInstance::SetAwake)
			.def("get_lua_awake", &CManchasSystemInstance::GetLuaAwake)
			.def("set_type", &CManchasSystemInstance::SetType)
	];

	//LINE RENDER
	module(m_LS)[
		class_<CLineRenderer, CRenderableObject>("CLineRenderer")
			.def("get_pos_inicial", &CLineRenderer::GetLuaPosInciial)
			.def("get_pos_final", &CLineRenderer::GetLuaPosFinal)
			.def("get_num_puntos", &CLineRenderer::GetLuaNumPuntos)
			.def("get_num_offsetX", &CLineRenderer::GetLuaOffsetX)
			.def("get_num_offsetY", &CLineRenderer::GetLuaOffsetY)
			.def("get_size", &CLineRenderer::GetLuaSize)
			.def("get_size_offset", &CLineRenderer::GetLuaSizeOffset)
			.def("get_color", &CLineRenderer::GetLuaColor)
	];

// GUI----------------------------------------------------------------------------------------------
	module(m_LS)[	
		class_<CGUIManager>("CGUIManager")
			.def("do_button", &CGUIManager::DoButton)
			.def("do_slider", &CGUIManager::DoSlider)
			.def("do_panel", &CGUIManager::DoPanel)
			.def("do_text", &CGUIManager::FillCommandQueueWithText)
			.enum_("gui_anchor")[
				value("top", CGUIManager::GUIAnchor::TOP),
				value("mid", CGUIManager::GUIAnchor::MID),
				value("bottom", CGUIManager::GUIAnchor::BOTTOM),
				value("left", CGUIManager::GUIAnchor::LEFT),
				value("center", CGUIManager::GUIAnchor::CENTER),
				value("right", CGUIManager::GUIAnchor::RIGHT),

				value("top_left", CGUIManager::GUIAnchor::TOP_LEFT),
				value("top_center", CGUIManager::GUIAnchor::TOP_CENTER),
				value("top_right", CGUIManager::GUIAnchor::TOP_RIGHT),
				value("mid_left", CGUIManager::GUIAnchor::MID_LEFT),
				value("mid_center", CGUIManager::GUIAnchor::MID_CENTER),
				value("mid_right", CGUIManager::GUIAnchor::MID_RIGHT),
				value("bottom_left", CGUIManager::GUIAnchor::BOTTOM_LEFT),
				value("bottom_center", CGUIManager::GUIAnchor::BOTTOM_CENTER),
				value("bottom_right", CGUIManager::GUIAnchor::BOTTOM_RIGHT)
			]
			.enum_("gui_coord_type")[
				value("gui_absolute", CGUIManager::GUICoordType::GUI_ABSOLUTE),
				value("gui_relative", CGUIManager::GUICoordType::GUI_RELATIVE),
				value("gui_relative_width", CGUIManager::GUICoordType::GUI_RELATIVE_WIDTH),
				value("gui_relative_height", CGUIManager::GUICoordType::GUI_RELATIVE_HEIGHT)
			]
	];
	module(m_LS)[
		class_<CGUIPosition>("CGUIPosition")
			.def(constructor<float, float, float, float, CGUIManager::GUIAnchor, CGUIManager::GUICoordType, CGUIManager::GUICoordType>())
	];
	module(m_LS)[
		class_<CSliderResult>("CSliderResult")
			.def(constructor<float, float>())
			.def_readwrite("real", &CSliderResult::m_Real)
			.def_readwrite("temp", &CSliderResult::m_Temp)
	];

// NETWORK------------------------------------------------------------------------------------------
	
	
// PHYSX--------------------------------------------------------------------------------------------

	module(m_LS)[
		class_<RaycastData>("RaycastData")
			.def(constructor<>())
			.def_readonly("position", &RaycastData::position)
			.def_readonly("normal", &RaycastData::normal)
			.def_readonly("distance", &RaycastData::distance)
			.def_readonly("actor_name", &RaycastData::actorname)
	];
	
	// PhysxManager-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CPhysXManager>("CPhysXManager")
			.def("get_texture", &CTextureManager::GetTexture)
			.def("reload", &CTextureManager::Reload)
			.def("enable_trigger", &CPhysXManager::EnableTrigger)
			.def("enable_object", &CPhysXManager::EnableObject)
			.def("disable_physics", &CPhysXManager::DisablePhysics)
			.def("create_character_controller", &CPhysXManager::CreateCharacterController)
			.def("character_controller_move", &CPhysXManager::CharacterControllerMove)
			.def("character_controller_warp", &CPhysXManager::CharacterControllerWarp)
			.def("character_controller_teleport", &CPhysXManager::CharacterControllerTeleport)
			.def("register_material", &CPhysXManager::RegisterMaterial)
			.def("raycast", &CPhysXManager::Raycast)
			.def("render", &CPhysXManager::Render)
			.def("get_character_controler_pos", &CPhysXManager::GetCharacterControllersPosition)
			.def("get_character_controler_lua_pos_x", &CPhysXManager::GetCharacterControllersPositionX)
			.def("get_character_controler_lua_pos_y", &CPhysXManager::GetCharacterControllersPositionY)
			.def("get_character_controler_lua_pos_z", &CPhysXManager::GetCharacterControllersPositionZ)
			.def("set_character_controller_height", &CPhysXManager::SetCharacterControllersHeight)
			.def("change_rigid_dynamic_actor_group", &CPhysXManager::ChangeRigidDynamicActorPhysxGroup)
			.def("change_rigid_static_actor_group", &CPhysXManager::ChangeRigidStaticActorPhysxGroup)
			.def("remove_actor", &CPhysXManager::RemoveActor)
			.def("change_actor_name", &CPhysXManager::ChangeActorName)
	];

	
// SOUND--------------------------------------------------------------------------------------------
	module(m_LS)[
		class_<SoundEvent>("SoundEvent")
			.def(constructor<>())
			.def_readwrite("event_name", &SoundEvent::eventName)
	];

	module(m_LS)[
		class_<SoundSwitch>("SoundSwitch")
			.def(constructor<>())
			.def_readwrite("switch_name", &SoundSwitch::switchName)
	];

	module(m_LS)[
		class_<SoundSwitchValue>("SoundSwitchValue")
			.def(constructor<>())
			.def_readwrite("sound_switch", &SoundSwitchValue::soundSwitch)
			.def_readwrite("value_name", &SoundSwitchValue::valueName)
	];

	module(m_LS)[
		class_<SoundRTPC>("SoundRTPC")
			.def(constructor<>())
			.def_readwrite("rtpc_name", &SoundRTPC::RTPCName)
	];

	module(m_LS)[
		class_<SoundState>("SoundState")
			.def(constructor<>())
			.def_readwrite("state_name", &SoundState::stateName)
	];

	module(m_LS)[
		class_<SoundStateValue>("SoundStateValue")
			.def(constructor<>())
			.def_readwrite("sound_state", &SoundStateValue::soundState)
			.def_readwrite("value_name", &SoundStateValue::valueName)
	];

	module(m_LS)[
		class_<ISoundManager>("ISoundManager")
			.scope[
				def("instantiate_sound_manager", &ISoundManager::InstantiateSoundManager)
			]
			.def("set_path", &ISoundManager::SetPath)
			.def("update", &ISoundManager::Update)
			.def("load", &ISoundManager::Load)
			.def("reload", &ISoundManager::Reload)
			.def("load_sound_bank", &ISoundManager::LoadSoundBank)
			.def("unload_sound_bank", &ISoundManager::UnloadSoundBank)
			.def("register_speaker", &ISoundManager::RegisterSpeaker)
			.def("unregister_speaker", &ISoundManager::UnregisterSpeaker)
			.def("play_event", (void(ISoundManager::*)(const SoundEvent&)) &ISoundManager::PlayEvent)
			.def("play_event", (void(ISoundManager::*)(const SoundEvent&, const std::string&)) &ISoundManager::PlayEvent)
			.def("play_event", (void(ISoundManager::*)(const SoundEvent&, const C3DElement*)) &ISoundManager::PlayEvent)
			.def("set_switch", (void(ISoundManager::*)(const SoundSwitchValue&)) &ISoundManager::SetSwitch)
			.def("set_switch", (void(ISoundManager::*)(const SoundSwitchValue&, const std::string&)) &ISoundManager::SetSwitch)
			.def("set_switch", (void(ISoundManager::*)(const SoundSwitchValue&, const C3DElement*)) &ISoundManager::SetSwitch)
			.def("broadcast_rtpc_value", (void(ISoundManager::*)(const SoundRTPC&, float)) &ISoundManager::BroadcastRTPCValue)
			.def("set_rtpc_value", (void(ISoundManager::*)(const SoundRTPC&, float)) &ISoundManager::SetRTPCValue)
			.def("set_rtpc_value", (void(ISoundManager::*)(const SoundRTPC&, float, const std::string&)) &ISoundManager::SetRTPCValue)
			.def("set_rtpc_value", (void(ISoundManager::*)(const SoundRTPC&, float, const C3DElement*)) &ISoundManager::SetRTPCValue)
			.def("broadcast_state", &ISoundManager::BroadcastState)
	];

// ARTIFICIAL INTELLIGENCE--------------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CAStar>("CAStar")
			.def(constructor<>())
			.def(constructor<const std::string&>())
			.def("load_map", &CAStar::LoadMap)
			.def("destroy_map", &CAStar::DestroyMap)
			.def("search_for_path", &CAStar::SearchForPath)
			.def("get_point", &CAStar::GetPoint)
			.def("get_patrol_point", &CAStar::GetPatrolPoint)
			.def("get_total_patrol_nodes", &CAStar::GetTotalPatrolNodes)
			.scope[
				class_<CAStar::TNode>("TNode")
					.def_readwrite("name", &CAStar::TNode::name)
					.def_readwrite("position", &CAStar::TNode::position),
				class_<CAStar::TNodePatrol>("TNodePatrol")
					.def_readwrite("node", &CAStar::TNodePatrol::node)
					.def_readwrite("wait", &CAStar::TNodePatrol::wait)
					.def_readwrite("time_to_wait", &CAStar::TNodePatrol::time_to_wait)
			]
	];

	RegisterTemplatedMapManager<CAStar>(m_LS);

	module(m_LS)[
		class_<CAStarManager, CTemplatedMapManager<CAStar>>("CStarManager")
			.def(constructor<>())
			.def("load", &CManchasManager::Load)
			.def("reload", &CManchasManager::Reload)
	];

// VIDEO
	module(m_LS)[
		class_<IVideoManager>("IVideoManager")
			.def("load_clip", &IVideoManager::LoadClip)
			.def("render_screen_clip", &IVideoManager::RenderSceenClip)
			.def("clear_clip", &IVideoManager::ClearClip)
			.def("play_clip", &IVideoManager::PlayClip)
			.def("restart_clip", &IVideoManager::ReloadClip)
	];

// VIDEOGAME----------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CApplication>("CApplication")
			.def(constructor<CContextManager*>())
			.def("update", &CApplication::Update)
			.def("render", &CApplication::Render)
			.def("init", &CApplication::Init)
	];

	module(m_LS)[
		class_<CGamePlayManager>("CGamePlayManager")
			.def(constructor<>())
			.def("add_component", &CGamePlayManager::AddComponent)
			.def("destroy", &CGamePlayManager::Destroy)
			.def("clear", &CGamePlayManager::Clear)
			.def("size", &CGamePlayManager::Size)
			.def("get_component",&CGamePlayManager::GetComponent)
	];
}

/*void OnEnterEvent(CEvent *Event)
{
	RunCode("on_enter_player_on_event_"+Event->GetName()+"()");
	RunCode("on_enter_player_on_event_event_1()");
}*/

