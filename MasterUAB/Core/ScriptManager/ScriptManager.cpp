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
#include "Components\UABComponent.h"
#include "Components\UABComponentManager.h"
#include "Components\ScriptedComponent.h"

#include "Engine\UABEngine.h"

#include "InputManager\InputManager.h"
#include "InputManager\InputManagerImplementation.h"

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
#include "Camera\SphericalCameraController.h"

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

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "DebugRender.h"

#include "PhysXManager\PhysXManager.h"

#include "Application.h"
#include "DebugHelper\DebugHelper.h"

#include "XML\XMLTreeNode.h"
#include "Utils.h"

#include <string>
#include <vector>
#include <map>

#include <typeinfo.h>

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


//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		UtilsLog(l_Str);
		//assert(!"must be log");
	}
}

//Para ejecutar un fichero de código LUA
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
			.def("get_map", &CTemplatedMapManager<T>::GetResourcesMap)
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
			.def("get_map", &CTemplatedVectorMapManager<T>::GetResourcesMap)
			.def("get_vector", &CTemplatedVectorMapManager<T>::GetResourcesVector)
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
			.def(constructor<const CXMLTreeNode&>())
			.def(constructor<const std::string&>())
			.def("set_name", &CNamed::SetName)
			.def("get_name", &CNamed::GetName)
	];

	luabind::module(m_LS) [ luabind::def("utils_log", &UtilsLog) ];

	module(m_LS)[
		class_<Vect3f>("Vect3f")
			.def(constructor<float, float, float>())
	];
	
// BASE------------------------------------------------------------------------------------------------

	// 3DElement---------------------------------------------------------------------------------------
	module(m_LS)[
		class_<C3DElement>("C3DElement")
			.def(constructor<>())
			.def(constructor<const Vect3f&>())
			.def(constructor<const Vect3f&, float, float, float>())
			.def(constructor<float, float, float>())
			.def(constructor<const CXMLTreeNode&>())
			.def("set_position", &C3DElement::SetPosition)
			.def("get_position", &C3DElement::GetPosition)
			.def("get_prev_position", &C3DElement::GetPrevPosition)
			.def("get_yaw", &C3DElement::GetYaw)
			.def("get_pitch", &C3DElement::GetPitch)
			.def("get_roll", &C3DElement::GetRoll)
			.def("set_yaw", &C3DElement::SetYaw)
			.def("set_pitch", &C3DElement::SetPitch)
			.def("set_roll", &C3DElement::SetRoll)
			.def("set_yaw_pitch_roll", &C3DElement::SetYawPitchRoll)
			.def("set_scale", &C3DElement::SetScale)
			.def("get_scale", &C3DElement::GetScale)
			.def("render", &C3DElement::Render)
			.def("get_transform", &C3DElement::GetTransform)
			.def("get_visible", &C3DElement::GetVisible)
			.def("set_visible", &C3DElement::SetVisible)
	];

	module(m_LS)[
		class_<CUABComponent, CNamed>("CUABComponent")
			.def(constructor<const std::string &, CRenderableObject *>())
			.def("Update", &CUABComponent::Update)
			.def("render", &CUABComponent::Render)
			.def("render_debug", &CUABComponent::RenderDebug)
	];


	module(m_LS)[
		class_<CUABComponentManager>("CUABComponentManager")
			.def("get_resource", &CUABComponentManager::GetResource)
			.def("add_resource", &CUABComponentManager::AddResource)
	];

	luabind::module(m_LS) [ luabind::def("create_scripted_component", &CreateScriptedComponent) ];


	module(m_LS)[
		class_<CEmptyPointerClass>("CEmptyPointerClass")
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
			.def("get_static_mesh_manager", &CUABEngine::GetStaticMeshManager)
			.def("get_layer_manager", &CUABEngine::GetLayerManager)
			.def("get_material_manager", &CUABEngine::GetMaterialManager)
			.def("get_effect_manager", &CUABEngine::GetEffectManager)
			.def("get_light_manager", &CUABEngine::GetLightManager)
			.def("get_render_manager", &CUABEngine::GetRenderManager)
			.def("get_animated_models_manager", &CUABEngine::GetAnimatedModelsManager)
			.def("get_script_manager", &CUABEngine::GetScriptManager)
			.def("get_camera_controller_manager", &CUABEngine::GetCameraControllerManager)
			.def("get_physX_manager", &CUABEngine::GetPhysXManager)
			.def("get_cinematic", &CUABEngine::GetCinematic)
			.def("get_level_loaded", &CUABEngine::GetLevelLoaded)
			.def("load_level_xml", &CUABEngine::LoadLevelXML)
			.scope[
				def("get_instance", &CUABEngine::GetInstance)
			]
			.def("destroy", &CUABEngine::Destroy)
			.def("init", &CUABEngine::Init)
	];

	// InputManager-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CInputManager>("CInputManager")
			.def("is_action_active", &CInputManager::IsActionActive)
			.def("get_axis", &CInputManager::GetAxis)
			.def("get_cursor", &CInputManager::GetCursor)
			.def("get_cursor_movement", &CInputManager::GetCursorMovement)
			.def("has_focus", &CInputManager::HasFocus)
			.def("set_current_input_manager", &CInputManager::SetCurrentInputManager)
			.scope[
				def("get_input_manager", &CInputManager::GetInputManager)
			]
			.def("reload", &CInputManager::reload)
	];

	module(m_LS)[
		class_<CInputManagerImplementation>("CInputManagerImplementation")
			.def(constructor<HWND>())
			.def("load_commands_from_file", &CInputManagerImplementation::LoadCommandsFromFile)
			.def("begin_frame", &CInputManagerImplementation::BeginFrame)
			.def("end_frame", &CInputManagerImplementation::EndFrame)
			.def("set_focus", &CInputManagerImplementation::SetFocus)
			.def("set_mouse_speed", &CInputManagerImplementation::SetMouseSpeed)
			.def("key_event_received", &CInputManagerImplementation::KeyEventReceived)
			.def("update_cursor", &CInputManagerImplementation::UpdateCursor)
			.def("update_cursor_movement", &CInputManagerImplementation::UpdateCursorMovement)
			.def("reload", &CInputManagerImplementation::reload)
	];
	

// GRAPHICS-----------------------------------------------------------------------------------------
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


	// RenderableObjects------------------------------------------------------------------------------
	module(m_LS)[
		class_<CRenderableObject, bases<C3DElement, CNamed>>("CRenderableObject")
			.def("update", &CRenderableObject::Update)
			.def("render", &CRenderableObject::Render)
			.def("get_component_manager", &CRenderableObject::GetComponentManager)
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
			.def("add_mesh_instance", (CRenderableObject*(CRenderableObjectsManager::*)(CXMLTreeNode&))&CRenderableObjectsManager::AddMeshInstance)
			.def("add_mesh_instance", (CRenderableObject*(CRenderableObjectsManager::*)(const std::string &, const std::string&, const Vect3f&, const float, const float, const float, const float, const bool))&CRenderableObjectsManager::AddMeshInstance)
			.def("add_animated_instance_model", (CRenderableObject*(CRenderableObjectsManager::*)(CXMLTreeNode&))&CRenderableObjectsManager::AddAnimatedInstanceModel)
			.def("add_animated_instance_model", (CRenderableObject*(CRenderableObjectsManager::*)(const std::string&, const std::string&, const Vect3f&))&CRenderableObjectsManager::AddAnimatedInstanceModel)
			.def("get_resource", &CRenderableObjectsManager::GetResource)
			//.def("clean_up", &CRenderableObjectsManager::CleanUp)
			.def("reload", &CRenderableObjectsManager::Reload)
			.def("load", &CRenderableObjectsManager::Load)
			//.def("get_instance", &CRenderableObjectsManager::GetInstance)
	];

	// TODO Añadir LayerManager
	module(m_LS)[
		class_<CLayerManager, CTemplatedVectorMapManager<CRenderableObjectsManager>>("CLayerManager")
			.def(constructor<>())
			.def("get_resource", &CLayerManager::GetResource)
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
			.def(constructor<CXMLTreeNode&>())
			.def("initialize", &CAnimatedInstanceModel::Initialize)
			.def("render", &CAnimatedInstanceModel::Render)
			.def("update", &CAnimatedInstanceModel::Update)
			.def("destroy", &CAnimatedInstanceModel::Destroy)
			.def("execute_action", &CAnimatedInstanceModel::ExecuteAction)
			.def("remove_action", &CAnimatedInstanceModel::RemoveAction)
			.def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
			.def("clear_cycle", &CAnimatedInstanceModel::ClearCycle)
			.def("is_cycle_animation_active", &CAnimatedInstanceModel::IsCycleAnimationActive)
			.def("is_action_animation_active", &CAnimatedInstanceModel::IsActionAnimationActive)
			.def("get_actual_cycle_animation", &CAnimatedInstanceModel::GetActualCycleAnimation)
			.def("get_actual_action_animation", &CAnimatedInstanceModel::GetActualActionAnimation)
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
		class_<CCameraController>("CCameraController")
			//.def(constructor<>())
			.def("set_camera", &CCameraController::SetCamera)
			.def("add_yaw", &CCameraController::AddYaw)
			.def("add_pitch", &CCameraController::AddPitch)
			.def("set_yaw", &CCameraController::SetYaw)
			.def("get_yaw", &CCameraController::GetYaw)
			.def("set_pitch", &CCameraController::SetPitch)
			.def("get_pitch", &CCameraController::GetPitch)
			.def("get_right", &CCameraController::GetRight)
			.def("get_up", &CCameraController::GetUp)
			.def("get_position", &CCameraController::GetPosition)
			.def("set_position", &CCameraController::SetPosition)
			.def("update", &CCameraController::Update)
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
	];

	module(m_LS)[
		class_<CCameraInfo>("CCameraInfo")
			.def(constructor<>())
			.def(constructor<const Vect3f, const Vect3f, const Vect3f, float, float, float>())
			.def(constructor<CXMLTreeNode&>())
	];

	module(m_LS)[
		class_<CCameraKey>("CCameraKey")
			.def(constructor<CCameraInfo&, float>())
	];

	module(m_LS)[
		class_<CCameraKeyController, CCameraController>("CCameraKeyController")
			.def(constructor<CXMLTreeNode&>())
			.def("update", &CCameraKeyController::Update)
			.def("set_current_time", &CCameraKeyController::SetCurrentTime)
			.def("reset_time", &CCameraKeyController::ResetTime)
			.def("get_total_time", &CCameraKeyController::GetTotalTime)
			.def("is_cycle", &CCameraKeyController::IsCycle)
			.def("set_cycle", &CCameraKeyController::SetCycle)
			.def("is_reverse", &CCameraKeyController::IsReverse)
			.def("set_reverse", &CCameraKeyController::SetReverse)
			.def("set_camera", &CCameraKeyController::SetCamera)			
	];

	module(m_LS) [
		class_<CFPSCameraController, CCameraController>("CFPSCameraController")
			.def(constructor<>())
			.def("move", &CFPSCameraController::Move)
			.def("set_camera", &CFPSCameraController::SetCamera)
			.def("add_yaw", &CFPSCameraController::AddYaw)
			.def("add_pitch",&CFPSCameraController::AddPitch)
			.def("get_direction", &CFPSCameraController::GetDirection)
	];

	module(m_LS) [
		class_<CFrustum>("CFrustum")
			.def("update", &CFrustum::Update)
			.def("sphere_visible", &CFrustum::SphereVisible)
			.def("box_visible",&CFrustum::BoxVisible)
	];

	module(m_LS) [
		class_<CSphericalCameraController, CCameraController>("CSphericalCameraController")
			.def(constructor<>())
			.def("add_zoom", &CSphericalCameraController::AddZoom)
			.def("set_zoom", &CSphericalCameraController::SetZoom)
			.def("set_camera", &CSphericalCameraController::SetCamera)
			.def("get_direction", &CSphericalCameraController::GetDirection)
			.def("move", &CSphericalCameraController::Move)
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
			.def("is_finished", &CCinematicPlayer::IsFinished)
			.def("get_duration", &CCinematicPlayer::GetDuration)
			.def("get_current_time", &CCinematicPlayer::GetTickCount)
			.def("on_restart_cycle", &CCinematicPlayer::OnRestartCycle)
	];

	module(m_LS)[
		class_<CCinematic, bases<CRenderableObject, CCinematicPlayer>>("CCinematic")
			.def(constructor<>())
			.def("load_xml", &CCinematic::LoadXML)
			.def("add_cinematic_object", &CCinematic::AddCinematicObject)
			.def("update", &CCinematic::Update)
			.def("render", &CCinematic::Render)
			.def("play",&CCinematic::Play)
			.def("stop",&CCinematic::Stop)
			.def("pause", &CCinematic::Pause)
			.def("on_restart_cycle", &CCinematic::OnRestartCycle)
	];

	module(m_LS)[
		class_<CCinematicObject, CCinematicPlayer>("CCinematicObject")
			.def(constructor<CXMLTreeNode&>())
			.def("is_ok", &CCinematicObject::IsOk)
			.def("add_cinematic_object_key_frame", &CCinematicObject::AddCinematicObjectKeyFrame)
			.def("update", &CCinematicObject::Update)
			.def("on_restart_cycle", &CCinematicObject::OnRestartCycle)
			.def("get_current_key", &CCinematicObject::GetCurrentKey)
	];

	module(m_LS)[
		class_<CCinematicObjectKeyFrame, C3DElement>("CCinematicObjectKeyFrame")
			.def(constructor<CXMLTreeNode&>())
			.def("get_key_frame_time",&CCinematicObjectKeyFrame::GetKeyFrameTime)
			.def("set_key_frame_time",&CCinematicObjectKeyFrame::SetKeyFrameTime)
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
			.def("draw", &CContextManager::Draw)
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
		class_<CEffect>("CEffect")
			.def(constructor<>())
			.def("update_parameters", &CEffect::UpdateParameters)
			.def("set_active", &CEffect::SetActive)
	];


	module(m_LS)[
		class_<CEffectParameters>("CEffectParameters")
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
			.def(constructor<const CXMLTreeNode&>())
			.def("load", &CEffectVertexShader::Load)
			.def("set_constant_buffer", &CEffectVertexShader::SetConstantBuffer)
			.def("reload", &CEffectVertexShader::Reload)
			.def("get_vertex_shader", &CEffectVertexShader::GetVertexShader)
			.def("get_vertex_layout", &CEffectVertexShader::GetVertexLayout)
			//.def("get_constant_buffer", &CEffectVertexShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectPixelShader, CEffectShader>("CEffectPixelShader")
			.def(constructor<const CXMLTreeNode&>())
			.def("reload", &CEffectPixelShader::Reload)
			.def("load", &CEffectPixelShader::Load)
			.def("set_constant_buffer", &CEffectPixelShader::SetConstantBuffer)
			.def("get_pixel_shader", &CEffectPixelShader::GetPixelShader)
			//.def("get_constant_buffer", &CEffectPixelShader::GetConstantBuffer)
	];

	module(m_LS)[
		class_<CEffectTechnique, CNamed>("CEffectTechnique")
			.def(constructor<CXMLTreeNode&>())
			.def("get_vertex_shader", &CEffectTechnique::GetVertexShader)
			.def("get_pixel_shader", &CEffectTechnique::GetPixelShader)
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
			.def("set_color", &CLight::SetColor)
			.def("set_intensity", &CLight::SetIntensity)
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
			.def("render", &CLightManager::Render)
			.def("reload", &CLightManager::Reload)
			.def("get_ambient_light", &CLightManager::GetAmbientLight)
	];

	module(m_LS)[
		class_<COmniLight,CLight>("COmniLight")
			.def(constructor<>())
			.def(constructor<CXMLTreeNode&>())
	];

	module(m_LS)[
		class_<CDirectionalLight, CLight>("CDirectionalLight")
			.def(constructor<>())
			.def(constructor<CXMLTreeNode&>())
			.def("get_direction", &CDirectionalLight::GetDirection)
			.def("set_direction", &CDirectionalLight::SetDirection)
			.def("render", &CDirectionalLight::Render)
	];

	module(m_LS)[
		class_<CSpotLight, CDirectionalLight>("CSpotLight")
			.def(constructor<>())
			.def(constructor<CXMLTreeNode&>())
			.def("get_angle", &CSpotLight::GetAngle)
			.def("set_angle", &CSpotLight::SetAngle)
			.def("get_fall_off", &CSpotLight::GetFallOff)
			.def("set_fall_off", &CSpotLight::SetFallOff)
	];

	// Materials--------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CMaterialParameter, CNamed>("CMaterialParameter")
			//.def(constructor<CMaterial*, CXMLTreeNode&, CMaterialParameter::TMaterialType>())
			.enum_("t_material_type")[
				value("float",CMaterialParameter::FLOAT),
				value("vect2f", CMaterialParameter::VECT2F),
				value("vect3f", CMaterialParameter::VECT3F),
				value("vect4f", CMaterialParameter::VECT4F)
			]
			.def("apply", &CMaterialParameter::Apply)
			.def("get_material_type", &CMaterialParameter::getMaterialType)
			.def("get_value_address", &CMaterialParameter::GetValueLuaAddress)
			.scope[
				def("get_type_from_string",&CMaterialParameter::GetTypeFromString)
			]
	];

	module(m_LS)[
		class_<CMaterial, CNamed>("CMaterial")
			.def(constructor<const CXMLTreeNode&>())
			.def("apply", &CMaterial::Apply)
			.def("get_next_parameter_adress", &CMaterial::GetNextParameterAddress)
			.def("get_parameters", &CMaterial::GetParameters, luabind::return_stl_iterator)
			.def("get_renderable_object_technique", &CMaterial::GetRenderableObjectTechnique)
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
	];

	// StaticMesh-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CInstanceMesh, CRenderableObject>("CInstanceMesh")
			.def(constructor<const CXMLTreeNode&>())
			.def(constructor<const std::string&, const std::string&>())
			.def("render", &CInstanceMesh::Render)
	];

	module(m_LS)[
		class_<CStaticMesh, CNamed>("CStaticMesh")
			.def(constructor<>())
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
		class_<CTextureManager, CTemplatedMapManager<CTexture>>("CTextureManager")
			.def(constructor<>())
			.def("get_texture", &CTextureManager::GetTexture)
			.def("reload", &CTextureManager::Reload)
	];


// GUI----------------------------------------------------------------------------------------------
	
	
// NETWORK------------------------------------------------------------------------------------------
	
	
// PHYSX--------------------------------------------------------------------------------------------
	
	// PhysxManager-------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CPhysXManager>("CPhysXManager")
			.def("get_texture", &CTextureManager::GetTexture)
			.def("reload", &CTextureManager::Reload)
			.def("create_character_controller", &CPhysXManager::CreateCharacterController)
			.def("character_controller_move", &CPhysXManager::CharacterControllerMove)		
			.def("register_material", &CPhysXManager::RegisterMaterial)
	];

	
// SOUND--------------------------------------------------------------------------------------------
	
	
// VIDEOGAME----------------------------------------------------------------------------------------
	module(m_LS)[
		class_<CApplication>("CApplication")
			.def(constructor<CContextManager*>())
			.def("switch_camera", &CApplication::SwitchCamera)
			.def("update", &CApplication::Update)
			.def("render", &CApplication::Render)
			.def("init", &CApplication::Init)
	];

}

/*void OnEnterEvent(CEvent *Event)
{
	RunCode("on_enter_player_on_event_"+Event->GetName()+"()");
	RunCode("on_enter_player_on_event_event_1()");
}*/

